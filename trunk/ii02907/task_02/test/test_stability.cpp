#include "StabilityAnalyzer.h"

#include <gtest/gtest.h>
#include <stdexcept>

TEST(StabilityAnalyzerTest, StableRealRoots) {
    const auto result = StabilityAnalyzer::analyze(0.5, 0.0, 1);
    EXPECT_TRUE(result.converged);
    EXPECT_TRUE(result.stable);
    ASSERT_EQ(result.roots.size(), 2U);
    EXPECT_NEAR(std::abs(result.roots[0]), 0.0, 1e-8);
    EXPECT_NEAR(std::abs(result.roots[1]), 0.5, 1e-8);
}

TEST(StabilityAnalyzerTest, StableComplexRoots) {
    const auto result = StabilityAnalyzer::analyze(0.0, -0.25, 1);
    EXPECT_TRUE(result.converged);
    EXPECT_TRUE(result.stable);
    for (const auto& root : result.roots) EXPECT_NEAR(std::abs(root), 0.5, 1e-8);
}

TEST(StabilityAnalyzerTest, DetectsUnstableRootOutsideUnitCircle) {
    const auto result = StabilityAnalyzer::analyze(1.2, 0.0, 1);
    EXPECT_TRUE(result.converged);
    EXPECT_FALSE(result.stable);
}

TEST(StabilityAnalyzerTest, UnitCircleBoundaryIsNotAsymptoticallyStable) {
    const auto result = StabilityAnalyzer::analyze(1.0, 0.0, 1);
    EXPECT_TRUE(result.converged);
    EXPECT_FALSE(result.stable);
}

TEST(StabilityAnalyzerTest, RejectsInvalidDelayAndCoefficients) {
    EXPECT_THROW(StabilityAnalyzer::analyze(0.5, 0.2, 0), std::invalid_argument);
    EXPECT_THROW(StabilityAnalyzer::analyze(0.5, 0.2, 101), std::invalid_argument);
    EXPECT_THROW(StabilityAnalyzer::analyze(std::numeric_limits<double>::infinity(), 0.2, 1),
                 std::invalid_argument);
}

TEST(StabilityAnalyzerTest, HigherOrderDelayedModelHasExpectedNumberOfRoots) {
    const auto result = StabilityAnalyzer::analyze(0.4, 0.1, 2);
    ASSERT_EQ(result.roots.size(), 3U);
    EXPECT_TRUE(result.converged);
}

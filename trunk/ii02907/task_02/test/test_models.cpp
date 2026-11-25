#include "Model15.h"
#include "Model27.h"
#include "Model39.h"

#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

struct Model15Case {
    double a1;
    double a2;
    double b;
    int delay;
    double u;
    std::vector<double> history;
    double expected;
};

class Model15ParameterizedTest : public ::testing::TestWithParam<Model15Case> {};

TEST_P(Model15ParameterizedTest, CalculatesNextSample) {
    const auto& c = GetParam();
    Model15 model(c.a1, c.a2, c.b, c.delay);
    EXPECT_NEAR(model.calculateNext(c.u, 0, c.history, 0.1), c.expected, 1e-12);
}

INSTANTIATE_TEST_SUITE_P(
    KnownValues, Model15ParameterizedTest,
    ::testing::Values(
        Model15Case{0.5, 0.2, 0.4, 1, 1.0, {0.0, 2.0}, 1.4},
        Model15Case{0.5, 0.2, 0.4, 1, 0.0, {1.0, 2.0}, 1.2},
        Model15Case{0.5, 0.2, 0.4, 2, 1.0, {1.0, 2.0}, 1.9},
        Model15Case{0.0, 0.0, 2.0, 1, 3.0, {0.0}, 6.0}));

TEST(Model15Test, RejectsNonPositiveDelay) {
    EXPECT_THROW(Model15(0.1, 0.2, 0.3, 0), std::invalid_argument);
}

TEST(Model27Test, RelaySwitchesPositiveAndNegative) {
    Model27 model(0.5, 2.0, 0.1);
    EXPECT_DOUBLE_EQ(model.calculateNext(1.0, 0, {0.0}, 0.1), 2.0);
    EXPECT_DOUBLE_EQ(model.calculateNext(-1.0, 1, {0.0, 0.0}, 0.1), -2.0);
}

TEST(Model27Test, HoldsRelayStateInsideHysteresisBand) {
    Model27 model(0.5, 2.0, 0.2);
    EXPECT_DOUBLE_EQ(model.calculateNext(0.0, 0, {0.0, 2.0}, 0.1), 3.0);
    EXPECT_DOUBLE_EQ(model.calculateNext(0.0, 0, {0.0, -2.0}, 0.1), -3.0);
}

TEST(Model27Test, RejectsNegativeEpsilon) {
    EXPECT_THROW(Model27(0.5, 1.0, -0.1), std::invalid_argument);
}

TEST(Model39Test, UsesEulerStepAndTanh) {
    Model39 model(2.0);
    EXPECT_NEAR(model.calculateNext(0.0, 0, {1.0}, 0.25), 1.0, 1e-12);
    EXPECT_NEAR(model.calculateNext(1.0, 0, {1.0}, 0.25),
                1.0 + 0.5 * std::tanh(1.0), 1e-12);
}

TEST(Model39Test, RejectsNonPositiveTimeStep) {
    Model39 model(1.0);
    EXPECT_THROW(model.calculateNext(1.0, 0, {0.0}, 0.0), std::invalid_argument);
}

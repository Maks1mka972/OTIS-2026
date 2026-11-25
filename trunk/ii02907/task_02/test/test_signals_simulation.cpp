#include "InputSignal.h"
#include "Model15.h"
#include "Model39.h"
#include "Simulation.h"

#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>

TEST(InputSignalTest, ConstantSignal) {
    EXPECT_DOUBLE_EQ(inputValue(InputType::Constant, 5, 2.5), 2.5);
}

TEST(InputSignalTest, ImpulseOnlyAtInitialStep) {
    EXPECT_DOUBLE_EQ(inputValue(InputType::Impulse, 0, 3.0), 3.0);
    EXPECT_DOUBLE_EQ(inputValue(InputType::Impulse, 1, 3.0), 0.0);
}

TEST(InputSignalTest, HarmonicSignal) {
    EXPECT_DOUBLE_EQ(inputValue(InputType::Harmonic, 0, 2.0), 0.0);
    EXPECT_NEAR(inputValue(InputType::Harmonic, 1, 2.0), 2.0 * std::sin(1.0), 1e-12);
}

struct SimulationCase {
    double a1;
    double b;
    double amplitude;
    int steps;
    double expectedFinal;
};

class SimulationParameterizedTest : public ::testing::TestWithParam<SimulationCase> {};

TEST_P(SimulationParameterizedTest, ProducesExpectedFinalValueForInputDataset) {
    const auto& c = GetParam();
    Model15 model(c.a1, 0.0, c.b, 1);
    const auto samples = simulate(
        model, [amplitude = c.amplitude](int) { return amplitude; },
        c.steps, 0.1);
    ASSERT_EQ(samples.size(), static_cast<std::size_t>(c.steps) + 1);
    EXPECT_NEAR(samples.back().y, c.expectedFinal, 1e-12);
}

INSTANTIATE_TEST_SUITE_P(
    StepCalculation, SimulationParameterizedTest,
    ::testing::Values(
        SimulationCase{0.0, 1.0, 2.0, 3, 6.0},
        SimulationCase{0.5, 1.0, 2.0, 3, 3.5},
        SimulationCase{0.2, 0.5, 4.0, 2, 2.4},
        SimulationCase{0.8, 0.0, 3.0, 3, 0.0}));

TEST(SimulationTest, ParameterSubstitutionInputIsUsed) {
    Model15 model(0.0, 0.0, 2.0, 1);
    const auto samples = simulate(model, [](int step) { return step + 1.0; }, 3, 0.5);
    ASSERT_EQ(samples.size(), 4U);
    EXPECT_DOUBLE_EQ(samples[1].u, 2.0);
    EXPECT_DOUBLE_EQ(samples[1].y, 2.0);
    EXPECT_DOUBLE_EQ(samples[3].y, 6.0);
}

TEST(SimulationTest, ZeroInputAndZeroInitialStateRemainAtRest) {
    Model39 model(0.8);
    const auto samples = simulate(model, [](int) { return 0.0; }, 5, 0.1);
    for (const auto& sample : samples) EXPECT_DOUBLE_EQ(sample.y, 0.0);
}

TEST(SimulationTest, RejectsInvalidArguments) {
    Model39 model(0.8);
    EXPECT_THROW(simulate(model, [](int) { return 0.0; }, -1, 0.1), std::invalid_argument);
    EXPECT_THROW(simulate(model, [](int) { return 0.0; }, 1, 0.0), std::invalid_argument);
    EXPECT_THROW(simulate(model, InputFunction{}, 1, 0.1), std::invalid_argument);
}

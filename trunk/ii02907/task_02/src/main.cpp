#include "InputSignal.h"
#include "Model15.h"
#include "Model27.h"
#include "Model39.h"
#include "Simulation.h"
#include "StabilityAnalyzer.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <string>

namespace {
std::string inputName(InputType type) {
    switch (type) {
        case InputType::Constant: return "constant";
        case InputType::Impulse: return "impulse";
        case InputType::Harmonic: return "harmonic";
    }
    return "unknown";
}
}

int main() {
    constexpr int steps = 40;
    constexpr double dt = 0.1;
    constexpr double amplitude = 1.0;

    Model15 model15(0.55, 0.20, 0.35, 2);
    Model27 model27(0.75, 0.40, 0.20);
    Model39 model39(0.80);

    const auto stability = StabilityAnalyzer::analyze(
        model15.a1(), model15.a2(), model15.delay());
    std::cout << "Model 1.5 characteristic roots (Z-plane):\n";
    for (const auto& root : stability.roots)
        std::cout << "  " << root << " |z|=" << std::abs(root) << '\n';
    if (!stability.stable)
        std::cout << "WARNING: Model 1.5 is unstable or stability analysis did not converge.\n";
    else
        std::cout << "Model 1.5 is stable.\n";

    const Model* models[] = {&model15, &model27, &model39};
    for (const Model* model : models) {
        std::cout << "\n=== " << model->name() << " ===\n";
        for (int choice = 1; choice <= 3; ++choice) {
            const auto type = static_cast<InputType>(choice);
            const auto samples = simulate(
                *model, [type](int step) { return inputValue(type, step, amplitude); },
                steps, dt);
            std::cout << inputName(type) << ": " << samples.size()
                      << " samples; final y=" << samples.back().y << '\n';
        }
    }
    return 0;
}

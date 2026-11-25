#include "InputSignal.h"
#include "Model15.h"
#include "Model27.h"
#include "Model39.h"
#include "Simulation.h"
#include "StabilityAnalyzer.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
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

void saveCsv(const std::string& filename, const std::vector<Sample>& samples) {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Cannot create file: " + filename);
    out << "step,tau,u,y\n" << std::setprecision(12);
    for (const auto& sample : samples)
        out << sample.step << ',' << sample.time << ','
            << sample.u << ',' << sample.y << '\n';
}

void printSamples(const std::vector<Sample>& samples) {
    std::cout << "step\t time\t\t u\t\t y\n";
    std::cout << std::fixed << std::setprecision(5);
    for (const auto& sample : samples)
        std::cout << sample.step << '\t' << sample.time << "\t\t "
                  << sample.u << "\t\t " << sample.y << '\n';
}

void runModel(const Model& model, int steps, double dt, double amplitude) {
    std::cout << "\n=== " << model.name() << " ===\n";
    std::string modelNumber;
    if (model.name() == "Model 1.5") modelNumber = "15";
    else if (model.name() == "Model 2.7") modelNumber = "27";
    else modelNumber = "39";

    for (int choice = 1; choice <= 3; ++choice) {
        const auto type = static_cast<InputType>(choice);
        const auto samples = simulate(
            model, [type, amplitude](int step) {
                return inputValue(type, step, amplitude);
            }, steps, dt);
        const std::string filename =
            "model" + modelNumber + "_" + inputName(type) + ".csv";
        saveCsv(filename, samples);
        std::cout << "\nInput: " << inputName(type)
                  << " | saved: " << filename << '\n';
        printSamples(samples);
    }
}
}

int main() {
    try {
        std::cout << "OTIS-2026 / Lab 2 / Variant 5\n";
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

        runModel(model15, steps, dt, amplitude);
        runModel(model27, steps, dt, amplitude);
        runModel(model39, steps, dt, amplitude);

        std::cout << "\nDone. Nine CSV files were written to the working directory.\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}

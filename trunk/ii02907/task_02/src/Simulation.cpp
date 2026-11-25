#include "Simulation.h"

#include <stdexcept>

std::vector<Sample> simulate(const Model& model, const InputFunction& input,
                              int steps, double dt) {
    if (steps < 0) throw std::invalid_argument("steps must be >= 0");
    if (dt <= 0.0) throw std::invalid_argument("dt must be > 0");
    if (!input) throw std::invalid_argument("input function is empty");

    std::vector<Sample> result;
    std::vector<double> history;
    result.reserve(static_cast<std::size_t>(steps) + 1);
    history.push_back(0.0);
    result.push_back({0, 0.0, input(0), 0.0});

    for (int step = 0; step < steps; ++step) {
        // Reuse the cached input for this step; evaluate each input step once.
        const double u = result.back().u;
        const double nextY = model.calculateNext(u, step, history, dt);
        history.push_back(nextY);
        result.push_back({step + 1, (step + 1) * dt, input(step + 1), nextY});
    }
    return result;
}

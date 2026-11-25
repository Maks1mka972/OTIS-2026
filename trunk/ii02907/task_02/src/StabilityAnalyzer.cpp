#include "StabilityAnalyzer.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
using Complex = std::complex<double>;

Complex evaluate(const std::vector<Complex>& coefficients, Complex z) {
    Complex value{};
    for (const auto& coefficient : coefficients)
        value = value * z + coefficient;
    return value;
}
}

StabilityResult StabilityAnalyzer::analyze(double a1, double a2, int delay) {
    if (delay < 1) throw std::invalid_argument("delay must be >= 1");
    if (!std::isfinite(a1) || !std::isfinite(a2))
        throw std::invalid_argument("coefficients must be finite");
    if (delay > 100) throw std::invalid_argument("delay must be <= 100");

    const int degree = delay + 1;
    std::vector<Complex> coefficients(static_cast<std::size_t>(degree) + 1);
    coefficients[0] = 1.0;
    coefficients[static_cast<std::size_t>(degree - delay)] = -a1;
    coefficients[static_cast<std::size_t>(degree)] = -a2;

    // If a2=0, the polynomial factors as z^delay * (z-a1).
    // Handle the repeated zero roots exactly instead of relying on iteration.
    if (a2 == 0.0) {
        std::vector<Complex> roots(static_cast<std::size_t>(degree), Complex{});
        roots[0] = a1;
        const bool stable = std::all_of(roots.begin(), roots.end(),
            [](const Complex& root) { return std::abs(root) < 1.0 - 1e-10; });
        return {roots, stable, true};
    }

    const double radius = 1.0 + std::max(std::abs(a1), std::abs(a2));
    const double pi = std::acos(-1.0);
    std::vector<Complex> roots(static_cast<std::size_t>(degree));
    for (int i = 0; i < degree; ++i) {
        const double angle = 2.0 * pi * (static_cast<double>(i) + 0.25) /
                             static_cast<double>(degree);
        roots[static_cast<std::size_t>(i)] =
            std::polar(radius, angle) + Complex(1e-7 * (i + 1), 1e-7 * (i + 1));
    }

    bool converged = false;
    constexpr int maxIterations = 3000;
    constexpr double tolerance = 1e-12;
    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        std::vector<Complex> nextRoots = roots;
        double largestChange = 0.0;
        for (int i = 0; i < degree; ++i) {
            Complex denominator = 1.0;
            for (int j = 0; j < degree; ++j) {
                if (i != j)
                    denominator *= roots[static_cast<std::size_t>(i)] -
                                  roots[static_cast<std::size_t>(j)];
            }
            if (std::abs(denominator) < 1e-24) continue;
            const Complex correction =
                evaluate(coefficients, roots[static_cast<std::size_t>(i)]) /
                denominator;
            nextRoots[static_cast<std::size_t>(i)] =
                roots[static_cast<std::size_t>(i)] - correction;
            largestChange = std::max(largestChange, std::abs(correction));
        }
        roots.swap(nextRoots);
        if (largestChange < tolerance) {
            converged = true;
            break;
        }
    }

    const bool stable = converged &&
        std::all_of(roots.begin(), roots.end(), [](const Complex& root) {
            return std::abs(root) < 1.0 - 1e-10;
        });
    return {roots, stable, converged};
}

bool StabilityAnalyzer::isStable(double a1, double a2, int delay) {
    return analyze(a1, a2, delay).stable;
}

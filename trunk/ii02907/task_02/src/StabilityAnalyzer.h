#pragma once

#include <complex>
#include <vector>

struct StabilityResult {
    std::vector<std::complex<double>> roots;
    bool stable{};
    bool converged{};
};

// Model 1.5: y[n+1] = a1*y[n] + a2*y[n-delay].
// Characteristic polynomial: z^(delay+1) - a1*z^delay - a2.
class StabilityAnalyzer {
public:
    static StabilityResult analyze(double a1, double a2, int delay);
    static bool isStable(double a1, double a2, int delay);
};

#pragma once

#include "Model.h"

#include <functional>
#include <vector>

using InputFunction = std::function<double(int)>;

std::vector<Sample> simulate(const Model& model, const InputFunction& input,
                             int steps, double dt);

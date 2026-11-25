#pragma once

enum class InputType { Constant = 1, Impulse = 2, Harmonic = 3 };

double inputValue(InputType type, int step, double amplitude);

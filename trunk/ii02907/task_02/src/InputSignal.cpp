#include "InputSignal.h"

#include <cmath>

double inputValue(InputType type, int step, double amplitude) {
    switch (type) {
        case InputType::Constant:
            return amplitude;
        case InputType::Impulse:
            return step == 0 ? amplitude : 0.0;
        case InputType::Harmonic:
            return amplitude * std::sin(static_cast<double>(step));
    }
    return 0.0;
}

#include "translate.h"

Translate::Translate(float dx, float dy, float dz) {
    // clang-format off
    matrix = {
        {1, 0, 0, dx},
        {0, 1, 0, dy},
        {0, 0, 1, dz},
        {0, 0, 0, 1}
    };
    // clang-format on
}

#pragma once
#include "transformation.h"

class Scale : public Transformation {
public:
    Scale() : Transformation() {}

    Scale(float x, float y, float z);

    // If this is a uniform scale, return the corresponding multiplier.
    // Otherwise, return value makes no sense.
    // @returns common scaling amount on three axes
    float amount();
};
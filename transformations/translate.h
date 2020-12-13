#pragma once
#include "transformation.h"

class Translate : public Transformation {
public:
    Translate() : Transformation() {}

    Translate(float dx, float dy, float dz);
};
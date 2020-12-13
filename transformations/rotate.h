#pragma once
#include "transformation.h"

class Rotate : public Transformation {
public:
    Rotate() : Transformation() {}

    // right-handed rotation of angle radians about axis
    Rotate(float angle, Vec3f axis);

protected:
    // @param angle radians
    Mat4 rotateX(float angle);
    // @param angle radians
    Mat4 rotateY(float angle);
    // @param angle radians
    Mat4 rotateZ(float angle);
};
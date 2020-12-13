#pragma once
#include "matrix.h"
#include "vector.h"

class Transformation {
public:
    // constructs an Identity transformation
    Transformation();

    Transformation operator*(const Transformation &other);
    Vec3f operator()(const Vec3f &vec);

    // @returns The normal transformation corresponding to this vertex
    // transformation
    Transformation normal();

protected:
    Transformation(const Mat4 &);
    Mat4 matrix;
};
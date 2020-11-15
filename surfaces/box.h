#pragma once

#include "triangle.h"
#include "vector.h"

class BoundingBox {
public:
    BoundingBox() {}
    BoundingBox(const std::vector<Triangle::Vertices> &triangles);

    float hit(const Ray &ray);
    TriangleBase *lastHit();

protected:
    enum Axis { x = 0, y = 1, z = 2 };

    Axis chooseAxis();

    // @returns MAXFLOAT_CONST if missed
    float boxT(const Ray &ray);

    Vec3f min, max;
    Axis divisionAxis;
    bool hasBox, hasChildren;
    BoundingBox *left, *right;
    std::vector<TriangleBase> triangles;
    union {
        TriangleBase *lastTriangle;
        BoundingBox *lastChild;
    };
};
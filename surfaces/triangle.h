#pragma once

#include "surface.h"

class TriangleBase : public Surface {
public:
    struct Vertices {
        Vec3f va, vb, vc;
    };

    TriangleBase() {}
    TriangleBase(const Vertices &v);

    // @returns -1 if no intersection, t otherwise
    float intersect(const Ray &ray) override;
    void normalAt(const Ray &ray, float t, Ray &out) override;

    Vertices v;
    Vec3f normal, col1, col2;
    float area, detL;
};

class Triangle : public TriangleBase {
public:
    Triangle(const Vertices &v, int materialId);

    int materialId;
};
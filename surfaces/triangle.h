#pragma once

#include "surface.h"

class TriangleBase : public Surface {
public:
    struct Vertices {
        union {
            struct {
                Vec3f va, vb, vc;
            };
            Vec3f arr[3];
        };
    };

    TriangleBase() {}
    TriangleBase(const Vertices &v);

    // @returns -1 if no intersection, t otherwise
    float intersect(const Ray &ray, Ray &normalOut) override;

    bool intersectsBefore(const Ray &ray, float t) override;

    Vertices v;

protected:
    Vec3f normal, col1, col2;
    float area, detL;
};

class Triangle : public TriangleBase {
public:
    Triangle(const Vertices &v, int materialId);

    int getMaterialId() const override;

protected:
    int materialId;
};
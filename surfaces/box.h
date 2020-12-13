#pragma once

#include "triangle.h"
#include "vector.h"

class BoundingBox {
public:
    BoundingBox() {}
    BoundingBox(const std::vector<Triangle::Vertices> &triangles,
                const std::vector<TexturedTriangle::TextureCoordinates>
                    texCoords = {});

    float hit(const Ray &ray, Ray &normalOut);
    float hit(const Ray &ray, Ray &normalOut, Vec2f &texCoordOut);

    bool intersectsBefore(const Ray &ray, float t);

protected:
    enum Axis { x = 0, y = 1, z = 2 };

    Axis chooseAxis();

    // @returns MAXFLOAT_CONST if missed
    float boxT(const Ray &ray);

    Vec3f min, max;
    bool hasBox, hasChildren;
    BoundingBox *left, *right;
    std::vector<TriangleBase> triangles;
    std::vector<TexturedTriangleBase> texturedTriangles;
};

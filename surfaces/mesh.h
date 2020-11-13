#pragma once

#include "surface.h"
#include "triangle.h"

class Mesh : public Surface {
public:
    Mesh(std::vector<Triangle::Vertices> faces, int materialId);

    float intersect(const Ray &ray) override;
    void normalAt(const Ray &ray, float t, Ray &out) override;

    // may need to call update() after adding faces. TODO: check after
    // implementing bounding boxes
    void addFace(Triangle::Vertices vertices);

    int materialId;

protected:
    std::vector<TriangleBase> triangles;
    Ray lastNormal;
};
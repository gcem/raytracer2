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

    // call after all faces are added
    void updateFaces();

    int getMaterialId() const override;

protected:
    struct BoundingBox {
        Vec3f min, max;
    };

    std::vector<TriangleBase> triangles;
    Ray lastNormal;
    int materialId;
    bool bruteForce = false;
    BoundingBox box;
};
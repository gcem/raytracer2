#pragma once

#include "surface.h"
#include "triangle.h"
#include "box.h"

class Mesh : public Surface {
public:
    Mesh(const std::vector<Triangle::Vertices> &faces, int materialId);

    float intersect(const Ray &ray) override;

    int getMaterialId() const override;

protected:
    int materialId;
    BoundingBox box;
};
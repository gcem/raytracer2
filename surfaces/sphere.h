#pragma once

#include "surface.h"

class Sphere : public Surface {
public:
    Sphere(const Vec3f &center, float radius, int materialId,
           int textureId = 0);

    float intersect(const Ray &ray, Ray &normalOut) override;

    bool intersectsBefore(const Ray &ray, float t) override;

    int getMaterialId() const override { return materialId; }
    int getTextureId() const override { return textureId; }

protected:
    Vec3f center;
    float radius;
    int materialId;
    int textureId = 0;
};
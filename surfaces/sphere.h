#pragma once

#include "surface.h"

class Sphere : public Surface {
public:
    Sphere(const Vec3f &center, float radius, int materialId);
    
    float intersect(const Ray &ray) override;
    
    int getMaterialId() const override;

protected:
    Vec3f center;
    float radius;
    int materialId;
};
#pragma once

#include "surface.h"

class Sphere : public Surface {
public:
    Sphere(const Vec3f &center, float radius, int materialId);
    
    float intersect(const Ray &ray) override;
    void normalAt(const Ray &ray, float t, Ray &out) override;
    
    int materialId;
protected:
    Vec3f center;
    float radius;
};
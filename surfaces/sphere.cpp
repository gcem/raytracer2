#include "sphere.h"

Sphere::Sphere(const Vec3f &center, float radius, int materialId)
    : center(center), radius(radius), materialId(materialId) {}

float Sphere::intersect(const Ray &ray) {
    auto centerPos = center - ray.origin;
    if (centerPos.dot(ray.direction) < 0)
        return -1;
    float centerDist = centerPos.norm();
    auto centerDirection = centerPos * (1 / centerDist);
    float perpRadius = ray.direction.sine(centerDirection) * centerDist;
    if (perpRadius > radius)
        return -1;

    float t = centerDirection.dot(ray.direction) * centerDist -
           sqrt(radius * radius - perpRadius * perpRadius);
    
    if (t > 0 && t < t_minT) {
        t_minT = t;
        t_matId = materialId;
        t_normal.origin = ray.origin + ray.direction * t;
        t_normal.direction = (t_normal.origin - center).normalize();
    }

    return t;
}

int Sphere::getMaterialId() const {
    return materialId;    
}

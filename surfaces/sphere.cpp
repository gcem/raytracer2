#include "sphere.h"

Sphere::Sphere(const Vec3f &center, float radius, int materialId)
    : center(center), radius(radius), materialId(materialId) {}

float Sphere::intersect(const Ray &ray, Ray &normalOut) {
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
    
    normalOut.origin = ray.origin + ray.direction * t;
    normalOut.direction = (normalOut.origin - center).normalize();

    return t;
}

int Sphere::getMaterialId() const {
    return materialId;    
}

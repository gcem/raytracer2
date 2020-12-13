#include "sphere.h"
#include <cmath>

Sphere::Sphere(const Vec3f &center, float radius, int materialId)
    : center(center), radius(radius), materialId(materialId) {}

float Sphere::intersect(const Ray &ray, Ray &normalOut) {
    auto centerPos = center - ray.origin;
    /* cameras can be inside spheres now
    if (centerPos.dot(ray.direction) < 0)
        return -1;
    */
    float centerDist = centerPos.norm();
    auto centerDirection = centerPos * (1 / centerDist);
    float perpRadius = ray.direction.sine(centerDirection) * centerDist;
    if (perpRadius > radius)
        return -1;

    float t = centerDirection.dot(ray.direction) * centerDist -
              sqrt(radius * radius - perpRadius * perpRadius);
    if (t < 0) // camera inside sphere
        t = centerDirection.dot(ray.direction) * centerDist +
            sqrt(radius * radius - perpRadius * perpRadius);

    normalOut.origin = ray.origin + ray.direction * t;
    normalOut.direction = (normalOut.origin - center) * (1 / radius);

    return t;
}

bool Sphere::intersectsBefore(const Ray &ray, float maxt) {
    auto centerPos = center - ray.origin;
    float centerDist = centerPos.norm();
    if (centerDist < radius - 0.0001f) // we are inside the sphere
        return false; // no shadow inside spheres. whatever.
    if (centerPos.dot(ray.direction) < 0)
        return false;
    auto centerDirection = centerPos * (1 / centerDist);
    float perpRadius = ray.direction.sine(centerDirection) * centerDist;
    if (perpRadius > radius)
        return false;
    float t = centerDirection.dot(ray.direction) * centerDist -
              sqrt(radius * radius - perpRadius * perpRadius);
    return t > 0 && t < maxt;
}

TexturedSphere::TexturedSphere(const Vec3f &center, float radius,
                               int materialId, int textureId, const Vec3f &up,
                               const Vec3f &right)
    : Sphere(center, radius, materialId), textureId(textureId), up(up),
      right(right) {
    z = right.cross(up);
}

float TexturedSphere::intersect(const Ray &ray, Ray &normalOut,
                                Vec2f &texCoordOut) {
    auto centerPos = center - ray.origin;
    /* cameras can be inside spheres now
    if (centerPos.dot(ray.direction) < 0)
        return -1;
    */
    float centerDist = centerPos.norm();
    auto centerDirection = centerPos * (1 / centerDist);
    float perpRadius = ray.direction.sine(centerDirection) * centerDist;
    if (perpRadius > radius)
        return -1;

    float t = centerDirection.dot(ray.direction) * centerDist -
              sqrt(radius * radius - perpRadius * perpRadius);
    if (t < 0) // camera inside sphere
        t = centerDirection.dot(ray.direction) * centerDist +
                   sqrt(radius * radius - perpRadius * perpRadius);

    normalOut.origin = ray.origin + ray.direction * t;
    normalOut.direction = (normalOut.origin - center) * (1 / radius);

    float upCos = up.dot(normalOut.direction);
    texCoordOut.y = acos(upCos) / M_PI;

    Vec3f xzComponent = normalOut.direction - up * upCos;
    float norm = xzComponent.norm();
    float angle = 0;
    if (norm != 0) {
        xzComponent /= norm;
        angle = acos(right.dot(xzComponent));
        float side = z.dot(xzComponent);
        if (side > 0)
            angle *= -1;
        angle += M_PI;
    }
    texCoordOut.x = angle / (M_PI * 2);
    return t;
}
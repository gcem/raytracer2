#pragma once

#include "surface.h"

class Sphere : public Surface {
public:
    Sphere(const Vec3f &center, float radius, int materialId);

    float intersect(const Ray &ray, Ray &normalOut) override;

    bool intersectsBefore(const Ray &ray, float t) override;

    int getMaterialId() const override { return materialId; }

protected:
    Vec3f center;
    float radius;
    int materialId;
};

class TexturedSphere : public Sphere {
public:
    // @param up result of transforming z axis
    // @param right result of transforming x axis
    TexturedSphere(const Vec3f &center, float radius, int materialId,
                   int textureId, const Vec3f &up, const Vec3f &right);

    float intersect(const Ray &ray, Ray &normalOut,
                    Vec2f &texCoordOut) override;
    int getTextureId() const override { return textureId; }

protected:
    int textureId;
    Vec3f up, right, z;
};
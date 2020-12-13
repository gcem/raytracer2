#pragma once

#include "ray.h"

class Surface {
public:
    // @returns -1 if no intersection, t otherwise
    virtual float intersect(const Ray &ray, Ray &normalOut) = 0;
    virtual float intersect(const Ray &ray, Ray &normalOut,
                            Vec2f &texCoordOut) { return -1; };

    virtual bool intersectsBefore(const Ray &ray, float t) = 0;

    // @returns 0 if not relevant (ids are one-indexed)
    virtual int getMaterialId() const { return 0; };

    // @returns 0 if not relevant (ids are one-indexed)
    virtual int getTextureId() const { return 0; };

protected:
    Surface() {}
};
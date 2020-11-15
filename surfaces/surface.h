#pragma once

#include "ray.h"

class Surface {
public:
    // @returns -1 if no intersection, t otherwise
    virtual float intersect(const Ray &ray, Ray &normalOut) = 0;

    // @returns 0 if not relevant (ids are one-indexed)
    virtual int getMaterialId() const;

protected:
    Surface() {}
};
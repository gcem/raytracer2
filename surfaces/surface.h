#pragma once

#include "ray.h"

extern thread_local float t_minT;
extern thread_local int t_matId, t_meshMatId;
extern thread_local Ray t_normal;

class Surface {
public:
    // @returns -1 if no intersection, t otherwise
    virtual float intersect(const Ray &ray) = 0;

    // @returns 0 if not relevant (ids are one-indexed)
    virtual int getMaterialId() const;

protected:
    Surface() {}
};
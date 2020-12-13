#pragma once

#include "parser.h"
#include "ray.h"
#include <vector>

struct IntersectionResult {
    // has no meaning if matId = 0
    float t;
    int matId;
    int texId;
    Vec2f texCoord;
};

Vec3f trace(const Ray &ray, int remainingReflections, Scene &scene);
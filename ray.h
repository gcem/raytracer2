#pragma once

#include "vector.h"
#include "camera.h"

#include <vector>

struct Camera;

struct Ray {
    Vec3f origin;
    Vec3f direction;
};

void castRays(Ray *out, const Camera &camera);
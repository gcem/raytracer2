#pragma once

#include "vector.h"

#include <string>

struct Camera {
    Vec3f position;
    Vec3f gaze;
    Vec3f up;
    Vec4f near_plane;
    float near_distance;
    int width, height;
    std::string image_name;
};
#pragma once

#include "parser.h"
#include "ray.h"
#include <vector>

void closestIntersection(const Ray &ray,
                         const std::vector<Surface *> &surfaces);

Vec3f trace(const Ray &ray, int remainingReflections, Scene &scene);
Vec3f computeColor(const Ray &ray, int remainingReflections, Scene &scene,
                   Ray normal, const Material &mat);
#pragma once

#include "parser.h"
#include "ray.h"
#include <vector>

std::pair<float, int>
closestIntersection(const Ray &ray, const std::vector<Surface *> &surfaces,
                    Ray &normalOut);
Vec3f trace(const Ray &ray, int remainingReflections, Scene &scene);
Vec3f computeColor(const Ray &ray, int remainingReflections, Scene &scene,
                   const Ray &normal, const Material &mat);
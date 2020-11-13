#include "trace.h"
#include "constants.h"
#include "parser.h"
#include <cmath>

// returned surface is nullptr if there is no intersection. in that case, t
// has no meaning
std::pair<float, Surface *>
closestIntersection(const Ray &ray, const std::vector<Surface *> &surfaces) {
    Surface *closest = nullptr;
    float mint = MAXFLOAT;
    for (auto surface : surfaces) {
        float t = surface->intersect(ray);
        if (t > 0 && t < mint) {
            closest = surface;
            mint = t;
        }
    }
    return {mint, closest};
}

Vec3f trace(const Ray &ray, int remainingReflections, Scene &scene) {
    auto closest = closestIntersection(ray, scene.surfaces);
    if (closest.second) {
        Surface *surface = closest.second;
        Ray normal;
        auto &mat = scene.materials[surface->getMaterialId()];
        surface->normalAt(ray, closest.first, normal);
        return computeColor(ray, remainingReflections, scene, normal, mat);
    }
    return scene.background_color;
}

Vec3f computeColor(const Ray &ray, int remainingReflections, Scene &scene,
                   const Ray &normal, const Material &mat) {
    Vec3f color = mat.ambient * scene.ambient_light;
    if (mat.diffuse.x || mat.diffuse.y || mat.diffuse.z || mat.specular.x ||
        mat.specular.y || mat.specular.z)
        for (auto &light : scene.point_lights) {
            Ray lightRay;

            lightRay.direction = (light.position - normal.origin);
            float cosine = normal.direction.dot(lightRay.direction);
            if (cosine < 0)
                continue; // light is behind the surface
            float lightDistance = lightRay.direction.norm();
            lightRay.direction /= lightDistance;
            cosine /= lightDistance;

            lightRay.origin =
                normal.origin + lightRay.direction * scene.shadow_ray_epsilon;

            auto closest = closestIntersection(lightRay, scene.surfaces);
            if (closest.second && closest.first < lightDistance)
                continue; // there is a surface in-between

            color += light.intensity * mat.diffuse *
                     (cosine / (lightDistance * lightDistance));
            color += light.intensity * mat.specular *
                     (std::pow((lightRay.direction - ray.direction)
                                   .normalize()
                                   .dot(normal.direction),
                               mat.phong_exponent) /
                      (lightDistance * lightDistance));
        }

    return color;
}
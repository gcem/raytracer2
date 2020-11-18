#include "trace.h"
#include "constants.h"
#include "parser.h"
#include <cmath>

// returned material id is 0 if there is no intersection. in that case, t
// has no meaning
// @returns { t, materialId }
std::pair<float, int>
closestIntersection(const Ray &ray, const std::vector<Surface *> &surfaces,
                    Ray &normalOut) {
    float mint = MAXFLOAT_CONST;
    Ray normal;
    int matId = 0;
    for (auto surface : surfaces) {
        float t = surface->intersect(ray, normal);
        if (t > 0 && t < mint) {
            normalOut = normal;
            matId = surface->getMaterialId();
            mint = t;
        }
    }
    return {mint, matId};
}

bool intersectsBefore(const Ray &ray, const std::vector<Surface *> &surfaces, float t) {
    for (auto surface : surfaces) {
        if (surface->intersectsBefore(ray, t))
            return true;
    }
    return false;
}

Vec3f trace(const Ray &ray, int remainingReflections, Scene &scene) {
    Ray normal;
    auto closest = closestIntersection(ray, scene.surfaces, normal);
    if (closest.second) {
        auto &mat = scene.materials[closest.second];
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

            if (intersectsBefore(lightRay, scene.surfaces, lightDistance))
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

    if (remainingReflections > 0 &&
        (mat.mirror.x || mat.mirror.y || mat.mirror.z)) {
        Ray reflectedRay;
        reflectedRay.origin =
            normal.origin +
            normal.direction *
                scene.shadow_ray_epsilon; // TODO: do not hit current object
        reflectedRay.direction =
            ray.direction -
            normal.direction * (2 * normal.direction.dot(ray.direction));
        color +=
            mat.mirror * trace(reflectedRay, remainingReflections - 1, scene);
    }

    return color;
}
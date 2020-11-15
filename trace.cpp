#include "constants.h"
#include "trace.h"
#include "parser.h"
#include "surface.h"
#include <cmath>

void closestIntersection(const Ray &ray,
                         const std::vector<Surface *> &surfaces) {
    t_minT = MAXFLOAT_CONST;
    for (auto surface : surfaces) {
        surface->intersect(ray);
    }
}

Vec3f trace(const Ray &ray, int remainingReflections, Scene &scene) {
    closestIntersection(ray, scene.surfaces);
    if (t_minT < 0 || t_minT == MAXFLOAT_CONST)
        return scene.background_color;

    return computeColor(ray, remainingReflections, scene, t_normal,
                        scene.materials[t_matId]);
}

Vec3f computeColor(const Ray &ray, int remainingReflections, Scene &scene,
                   Ray normal, const Material &mat) {
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

            closestIntersection(lightRay, scene.surfaces);
            if (t_minT > 0 && t_minT < lightDistance)
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
#include "trace.h"
#include "constants.h"
#include "parser.h"
#include <cmath>
#include <tuple>

Vec3f computeColor(const Ray &ray, int remainingReflections, Scene &scene,
                   const Ray &normal, const Material &mat,
                   Texture::Mode texMode, Vec3f texColor);

// returned material id is 0 if there is no intersection. in that case, t
// has no meaning.
// returned textureId is 0 if there is no intersection.
// @returns { t, materialId, textureId, textureCoordinates }
IntersectionResult closestIntersection(const Ray &ray,
                                       const std::vector<Surface *> &surfaces,
                                       Ray &normalOut) {
    IntersectionResult result{MAXFLOAT_CONST, 0, 0, {}};
    Ray normal;
    Vec2f tex;
    float t;
    for (auto surface : surfaces) {
        if (surface->getTextureId())
            t = surface->intersect(ray, normal, tex);
        else
            t = surface->intersect(ray, normal);

        if (t > 0 && t < result.t) {
            normalOut = normal;
            result.t = t;
            result.matId = surface->getMaterialId();
            result.texId = surface->getTextureId();
            if (result.texId)
                result.texCoord = tex;
        }
    }
    return result;
}

bool intersectsBefore(const Ray &ray, const std::vector<Surface *> &surfaces,
                      float t) {
    for (auto surface : surfaces) {
        if (surface->intersectsBefore(ray, t))
            return true;
    }
    return false;
}

Vec3f trace(const Ray &ray, int remainingReflections, Scene &scene) {
    Ray normal;
    auto closest = closestIntersection(ray, scene.surfaces, normal);
    if (closest.matId) {
        auto &mat = scene.materials[closest.matId];
        auto &texture = scene.textures[closest.texId];
        Vec3f texColor;
        if (closest.texId)
            texColor = texture.sample(closest.texCoord.x, closest.texCoord.y);
        return computeColor(ray, remainingReflections, scene, normal, mat,
                            texture.mode,
                            texColor); // texture mode is None if texId = 0
    }
    return scene.background_color;
}

Vec3f computeColor(const Ray &ray, int remainingReflections, Scene &scene,
                   const Ray &normal, const Material &mat,
                   Texture::Mode texMode, Vec3f texColor) {
    Vec3f color = mat.ambient * scene.ambient_light;
    if (texMode == Texture::Mode::TextureOnly)
        color += texColor;
    if (texMode != Texture::Mode::TextureOnly &&
            (mat.diffuse.x || mat.diffuse.y || mat.diffuse.z) ||
        mat.specular.x || mat.specular.y || mat.specular.z)
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

            if (texMode == Texture::Mode::Diffuse) {
                color += light.intensity * texColor / 255 *
                         (cosine / (lightDistance * lightDistance));
            } else if (texMode == Texture::Mode::DiffuseBlend) {
                color += light.intensity * (mat.diffuse + texColor / 255) /
                         2 * (cosine / (lightDistance * lightDistance));
            } else if (texMode == Texture::Mode::None)
                color += light.intensity * mat.diffuse *
                         (cosine / (lightDistance * lightDistance));
            // Texture::Mode::Replace is handled at the beginning

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
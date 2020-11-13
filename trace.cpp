#include "trace.h"
#include "parser.h"
#include <cmath>
#include <limits>

void trace(std::vector<Ray> &rays, const Scene &scene, unsigned char *image) {
    int pixels = rays.size();
    for (int i = 0, k = 0; i < pixels; i++) {
        const Ray &ray = rays[i];
        const Sphere *closestSph = 0;
        float mint = std::numeric_limits<float>::max();
        for (auto &sph : scene.spheres) {
            float t = sph.intersect(ray, scene);
            if (t > 0 && t < mint) {
                closestSph = &sph;
                mint = t;
            }
        }
        if (mint != std::numeric_limits<float>::max()) {
            image[k++] = mint * mint * mint;
            image[k++] = mint * mint * mint;
            image[k++] = mint * mint;
        }
        else {
            image[k++] = 0;
            image[k++] = 0;
            image[k++] = 0;
        }
    }
}

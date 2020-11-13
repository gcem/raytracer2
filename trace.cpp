#include "trace.h"
#include "parser.h"
#include "constants.h"
#include <cmath>

void trace(std::vector<Ray> &rays, Scene &scene, unsigned char *image) {
    int pixels = rays.size();
    for (int i = 0, k = 0; i < pixels; i++) {
        const Ray &ray = rays[i];
        const Surface *closest = nullptr;
        float mint = MAXFLOAT;
        for (auto surface : scene.surfaces) {
            float t = surface->intersect(ray);
            if (t > 0 && t < mint) {
                closest = surface;
                mint = t;
            }
        }
        if (closest) {
            auto &mat = scene.materials[closest->getMaterialId()];
            image[k++] = (closest - (Surface*) 1) % 255;
            image[k++] = (closest - (Surface*) 1) % 255;
            image[k++] = (closest - (Surface*) 1) % 255;
        }
        else {
            image[k++] = 0;
            image[k++] = 0;
            image[k++] = 0;
        }
    }
}

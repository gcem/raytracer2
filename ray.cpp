#include "ray.h"
#include "constants.h"
#include <thread>

void castGroup(Ray *out, Vec3f origin, Vec3f topLeft,
               Vec3f right, Vec3f up, int start, int end, int width) {
    topLeft = topLeft - up * start;
    int outi = start * width;

    for (int row = start; row < end; row++) {
        Vec3f currDir = topLeft;
        for (int col = 0; col < width; col++) {
            Ray &ray = out[outi++];
            ray.origin = origin;
            ray.direction = currDir.normalize();
            currDir = currDir + right;
        }
        topLeft = topLeft - up;
    }
}

void castRays(Ray *out, const Camera &camera) {
    Vec3f right = camera.gaze.cross(camera.up).normalize();
    Vec3f up = camera.up.normalize();
    Vec3f topLeft = camera.gaze * camera.near_distance +
                    right * camera.near_plane.left +
                    up * camera.near_plane.top;
    Vec3f currdir;
    right = right * ((camera.near_plane.right - camera.near_plane.left) /
                     (camera.width));
    up = up * ((camera.near_plane.top - camera.near_plane.bottom) /
               (camera.height));
    topLeft = topLeft + right * 0.5 - up * 0.5;

    int part = (camera.height + THREAD_CNT - 1) / THREAD_CNT;
    std::thread rayCasters[8];

    for (int i = 0; i < THREAD_CNT; i++) {
        rayCasters[i] = std::thread(
            castGroup, out, camera.position, topLeft, right, up, i * part,
            std::min((i + 1) * part, camera.height), camera.width);
    }

    for (int i = 0; i < THREAD_CNT; i++) {
        rayCasters[i].join();
    }
}

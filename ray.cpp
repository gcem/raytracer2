#include "ray.h"

void castRays(std::vector<Ray> &out, const Camera &camera) {
    out.resize(camera.width * camera.height);
    Vec3f right = camera.gaze.cross(camera.up).normalize();
    Vec3f up = camera.up.normalize();
    Vec3f leftdir = camera.gaze * camera.near_distance +
                    right * camera.near_plane.left +
                    up * camera.near_plane.top;
    Vec3f currdir;
    right = right * ((camera.near_plane.right - camera.near_plane.left) /
                     (camera.width));
    up = up * ((camera.near_plane.top - camera.near_plane.bottom) /
                   (camera.height));
    leftdir = leftdir + right * 0.5 - up * 0.5;

    int i = 0;
    for (int row = 0; row < camera.height; row++) {
        currdir = leftdir;
        for (int col = 0; col < camera.width; col++) {
            Ray &ray = out[i++];
            ray.origin = camera.position;
            ray.direction = currdir;
            currdir = currdir + right;
        }
        leftdir = leftdir - up;
    }
}

#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "trace.h"
#include <chrono>
#include <iostream>

#include "constants.h"
#include "ray.h"
#include <atomic>
#include <thread>

/**
 * note to my future self: I should actually have partitioned the space for
 * triangles and spheres too. this code partitions meshes only.
 *
 * however, it is already 9 pm sunday and I am very tired :(
 */

void render(Ray *rays, uint8_t **out, Scene *scene, const int width,
            const int height, std::atomic<int> *nextTile, const int tileCount,
            const int horizontalTiles) {
    int tile;
    for (;;) {
        tile = (*nextTile)++;
        if (tile >= tileCount)
            return;
        int firstCol = (tile % horizontalTiles) * TILE_WIDTH;
        int lastCol = std::min((tile % horizontalTiles + 1) * TILE_WIDTH,
                               width); // exclusive
        int line = (tile / horizontalTiles) * TILE_HEIGHT;
        int lastLine = std::min(line + TILE_HEIGHT, height);

        for (; line < lastLine; line++) {
            int rayi = line * width + firstCol;
            for (int col = firstCol, k = firstCol * 3; col < lastCol;
                 col++, rayi++) {
                auto color =
                    trace(rays[rayi], scene->max_recursion_depth, *scene);
                out[line][k++] = color.x > 255 ? 255 : std::roundf(color.x);
                out[line][k++] = color.y > 255 ? 255 : std::roundf(color.y);
                out[line][k++] = color.z > 255 ? 255 : std::roundf(color.z);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // Sample usage for reading an XML scene file
    Scene scene;

    scene.loadFromXml(argv[1]);

    for (auto camera : scene.cameras) {
        int size = camera.width * camera.height;
        uint8_t **image = new uint8_t *[camera.height];
        for (int i = 0; i < camera.height; i++) {
            image[i] = new uint8_t[camera.width * 3];
        }

        Ray *rays = new Ray[size];
        castRays(rays, camera);

        std::thread renderers[THREAD_CNT];
        std::atomic<int> nextTile;
        nextTile.store(0);
        int horizontalTiles = (camera.width + TILE_WIDTH - 1) / TILE_WIDTH;
        int verticalTiles = (camera.height + TILE_HEIGHT - 1) / TILE_HEIGHT;

        for (int i = 0; i < THREAD_CNT; i++) {
            renderers[i] = std::thread(
                render, rays, image, &scene, camera.width, camera.height,
                &nextTile, horizontalTiles * verticalTiles, horizontalTiles);
        }

        for (auto &thread : renderers) {
            thread.join();
        }
        write_ppm(camera.image_name,
                  image, camera.width, camera.height);

        for (int i = 0; i < camera.height; i++) {
            delete[] image[i];
        }
        delete[] image;
        delete[] rays;
    }
}

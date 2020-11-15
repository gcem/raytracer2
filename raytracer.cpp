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
            for (int col = firstCol, k = firstCol * 3; col < lastCol; col++, rayi++) {
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
    // TODO: delete
    auto start = std::chrono::system_clock::now();

    // Sample usage for reading an XML scene file
    Scene scene;

    scene.loadFromXml(argv[1]);

    auto afterload = std::chrono::system_clock::now();

    for (auto camera : scene.cameras) {
        int size = camera.width * camera.height;
        uint8_t **image = new uint8_t *[camera.height];
        for (int i = 0; i < camera.height; i++) {
            image[i] = new uint8_t[camera.width * 3];
        }

        Ray *rays = new Ray[size];
        castRays(rays, camera);

        std::thread renderers[8];
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

        auto writeBegin = std::chrono::system_clock::now();
        write_ppm("output/" + camera.image_name, // TODO: delete output/
                  image, camera.width, camera.height);
        auto writeEnd = std::chrono::system_clock::now();
        std::cout << "Wrote: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                         writeEnd - writeBegin)
                         .count()
                  << "\n";

        for (int i = 0; i < camera.height; i++) {
            delete[] image[i];
        }
        delete[] image;
        delete[] rays;
    }

    // TODO: delete
    auto end = std::chrono::system_clock::now();
    int64_t loading = std::chrono::duration_cast<std::chrono::milliseconds>(
                          afterload - start)
                          .count();
    int64_t rendering =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - afterload)
            .count();
    int64_t milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
    std::cout << "Loading: " << loading << "\nRendering: " << rendering
              << "\nTime: " << milliseconds << " milliseconds" << std::endl;
}

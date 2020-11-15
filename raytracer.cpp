#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "trace.h"
#include <chrono>
#include <iostream>

#include "constants.h"
#include "ray.h"
#include <thread>

void render(Ray *rays, uint8_t *out, int count, Scene *scene,
            int threadi) {
    for (int i = 0, k = 0; i < count; i++) {
        auto color = trace(rays[i], scene->max_recursion_depth, *scene);
        out[k++] = color.x > 255 ? 255 : std::roundf(color.x);
        out[k++] = color.y > 255 ? 255 : std::roundf(color.y);
        out[k++] = color.z > 255 ? 255 : std::roundf(color.z);
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
        uint8_t *image = new uint8_t[size * 3];
        Ray *rays = new Ray[size];
        castRays(rays, camera);

        int part = (size + THREAD_CNT - 1) / THREAD_CNT;
        std::thread renderers[8];

        for (int i = 0; i < THREAD_CNT; i++) {
            renderers[i] =
                std::thread(render, rays + i * part, image + i * part * 3,
                            std::min(part, size - i * part), &scene, i);
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

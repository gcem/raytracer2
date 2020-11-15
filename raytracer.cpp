#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "trace.h"
#include <chrono>
#include <iostream>

int main(int argc, char *argv[]) {
    // TODO: delete
    auto start = std::chrono::system_clock::now();

    // Sample usage for reading an XML scene file
    Scene scene;

    scene.loadFromXml(argv[1]);

    auto afterload = std::chrono::system_clock::now();

    for (auto camera : scene.cameras) {
        uint8_t *image =
            new uint8_t[camera.width * camera.height * 3];
        std::vector<Ray> rays;
        castRays(rays, camera);
        for (int i = 0, k = 0; i < camera.width * camera.height; i++) {
            auto color = trace(rays[i], scene.max_recursion_depth, scene);
            image[k++] = color.x > 255 ? 255 : std::roundf(color.x);
            image[k++] = color.y > 255 ? 255 : std::roundf(color.y);
            image[k++] = color.z > 255 ? 255 : std::roundf(color.z);
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

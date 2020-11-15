#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "trace.h"
#include <chrono>
#include <iostream>

typedef unsigned char RGB[3];

int main(int argc, char *argv[]) {
    // TODO: delete
    auto start = std::chrono::system_clock::now();

    // Sample usage for reading an XML scene file
    Scene scene;

    scene.loadFromXml(argv[1]);

    auto afterload = std::chrono::system_clock::now();

    for (auto camera : scene.cameras) {
        // TODO: delete
        int64_t *times = new int64_t[camera.width * camera.height];
        int64_t maxTime = 0;

        unsigned char *image =
            new unsigned char[camera.width * camera.height * 3];
        std::vector<Ray> rays;
        castRays(rays, camera);
        for (int i = 0, k = 0; i < camera.width * camera.height; i++) {
            auto before = std::chrono::system_clock::now();
            auto color = trace(rays[i], scene.max_recursion_depth, scene);
            auto after = std::chrono::system_clock::now();
            times[i] = (after - before).count();
            maxTime = std::max(maxTime, times[i]);

            k++;
            image[k++] = 0;
            image[k++] = 0;
            /*
            image[k++] = color.x > 255 ? 255 : std::roundf(color.x);
            image[k++] = color.y > 255 ? 255 : std::roundf(color.y);
            image[k++] = color.z > 255 ? 255 : std::roundf(color.z);
            */
        }

        for (int i = 0; i < camera.width * camera.height; i++) {
            image[i * 3] = times[i] * 255.f / maxTime;
        }

        write_ppm("output/" + camera.image_name, image, camera.width,
                  camera.height);
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

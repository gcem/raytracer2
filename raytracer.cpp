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

    for (auto camera : scene.cameras) {
        unsigned char *image =
            new unsigned char[camera.width * camera.height * 3];
        std::vector<Ray> rays;
        castRays(rays, camera);
        for (int i = 0, k = 0; i < camera.width * camera.height; i++) {
            auto color = trace(rays[i], scene.max_recursion_depth, scene);
            image[k++] = color.x > 255 ? 255 : std::roundf(color.x);
            image[k++] = color.y > 255 ? 255 : std::roundf(color.y);
            image[k++] = color.z > 255 ? 255 : std::roundf(color.z);
        }
        write_ppm("output/" + camera.image_name, image, camera.width,
                  camera.height);
        delete[] image;
    }

    // TODO: delete
    auto end = std::chrono::system_clock::now();
    int64_t milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
    std::cout << "Time: " << milliseconds << " milliseconds" << std::endl;
}

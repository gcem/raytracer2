#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "trace.h"
#include <iostream>

typedef unsigned char RGB[3];

int main(int argc, char *argv[]) {
    // Sample usage for reading an XML scene file
    Scene scene;

    scene.loadFromXml(argv[1]);

    for (auto camera : scene.cameras) {
        unsigned char *image =
            new unsigned char[camera.width * camera.height * 3];
        std::vector<Ray> rays;
        castRays(rays, camera);
        trace(rays, scene, image);
        // for (int i = 0, k = 0; i < camera.width * camera.height; i++) {
        //     image[k++] = (rays[i].direction.x + 10) * 10;
        //     image[k++] = (rays[i].direction.y + 10) * 10;
        //     image[k++] = (rays[i].direction.z + 11) * 0.001;
        // }
        write_ppm("output/" + camera.image_name, image, camera.width,
                  camera.height);
        delete[] image;
    }
}

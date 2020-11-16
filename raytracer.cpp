#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "trace.h"
#include <chrono>
#include <iostream>

#include "constants.h"
#include "ray.h"
#include <atomic>
#include <fstream>
#include <mutex>
#include <thread>

/**
 * note to my future self: I should actually have partitioned the space for
 * triangles and spheres too. this code partitions meshes only.
 *
 * however, it is already 9 pm sunday and I am very tired :(
 */

std::mutex writeFile;
int nextLine = 0;
extern std::ofstream outf;

void writeLines(uint8_t *out, int height, int width, bool *ready) {
    for (; nextLine < height; nextLine++) {
        int k = nextLine * width * 3;
        for (; k < width * 3; k++) {
            outf << (int)out[k] << ' ';
        }
        outf << '\n';
    }
}

void tryWriteLines(uint8_t *out, int height, int width, bool *ready) {
    if (writeFile.try_lock()) {
        for (; nextLine < height && ready[nextLine]; nextLine++) {
            int k = nextLine * width * 3;
            for (; k < (nextLine + 1) * width * 3; k++) {
                outf << (int)out[k] << ' ';
            }
            outf << '\n';
        }
        writeFile.unlock();
    }
}

void render(Ray *rays, uint8_t *out, Scene *scene, const int width,
            const int height, int firstLine, bool *ready) {
    for (int line = firstLine; line < height; line += THREAD_CNT) {
        int rayi = line * width;
        for (int col = 0, k = rayi * 3; col < width; col++, rayi++) {
            auto color =
                trace(rays[rayi], scene->max_recursion_depth, *scene);
            out[k++] = color.x > 255 ? 255 : std::roundf(color.x);
            out[k++] = color.y > 255 ? 255 : std::roundf(color.y);
            out[k++] = color.z > 255 ? 255 : std::roundf(color.z);
        }
        ready[line] = true;
        tryWriteLines(out, height, width, ready);
    }
}

int main(int argc, char *argv[]) {
    // TODO: delete
    auto start = std::chrono::system_clock::now();

    // Sample usage for reading an XML scene file
    Scene scene;

    scene.loadFromXml(argv[1]);

    for (auto camera : scene.cameras) {
        int size = camera.width * camera.height;
        uint8_t *image = new uint8_t[size * 3];
        bool *ready = new bool[camera.height]{};

        Ray *rays = new Ray[size];
        castRays(rays, camera);

        std::thread renderers[THREAD_CNT];

        write_ppm_header(camera.image_name, camera.width, camera.height);
        nextLine = 0;

        for (int i = 0; i < THREAD_CNT; i++) {
            renderers[i] = std::thread(render, rays, image, &scene,
                                       camera.width, camera.height, i, ready);
        }

        for (auto &thread : renderers) {
            thread.join();
        }

        writeLines(image, camera.height, camera.width, ready);
        outf.close();

        delete[] ready;
        delete[] image;
        delete[] rays;
    }

    // TODO: delete
    auto end = std::chrono::system_clock::now();
    int64_t milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
    std::cout << "Time: " << milliseconds << " milliseconds" << std::endl;
}

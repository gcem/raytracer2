#include "ppm.h"
#include "constants.h"
#include <fstream>
#include <sstream>
#include <thread>

void writeToBuffer(std::string *out, uint8_t *data, int start, int end,
                   int width) {
    out->reserve(width * (end - start) * 3 * 4);
    std::stringstream str;
    for (int row = start; row < end; row++) {
        int coli = row * width * 3;
        for (; coli < (row + 1) * width * 3 - 1; coli++) {
            str << (int)data[coli] << ' ';
        }
        str << (int)data[coli] << '\n';
        out->append(str.str());
        str.str(std::string());
        str.clear();
    }
}

void write_ppm(std::string filename, uint8_t *data, int width, int height) {
    std::ofstream outf;
    outf.open(filename, std::ofstream::out | std::ofstream::trunc);

    outf << "P3\n" << width << ' ' << height << "\n255\n";

    int part = (height + THREAD_CNT - 1) / THREAD_CNT;

    std::string buffers[THREAD_CNT * 2]; // align to 64 bytes
    std::thread bufferWriters[8];

    for (int i = 0; i < THREAD_CNT; i++) {
        bufferWriters[i] =
            std::thread(writeToBuffer, buffers + (i << 1), data, i * part,
                        std::min((i + 1) * part, height), width);
    }
    for (int i = 0; i < THREAD_CNT; i++) {
        bufferWriters[i].join();
        outf << buffers[i << 1];
    }

    outf.close();
}

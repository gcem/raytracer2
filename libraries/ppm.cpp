#include "ppm.h"
#include "constants.h"
#include <fstream>
#include <sstream>
#include <thread>

void writeToBuffer(std::string *out, uint8_t **data, int start, int end,
                   int width) {
    out->reserve(width * (end - start) * 3 * 4);
    std::stringstream str;
    for (int row = start; row < end; row++) {
        uint8_t *rowp = data[row];
        int coli = 0;
        for (; coli < width * 3 - 1; coli++) {
            str << (int)rowp[coli] << ' ';
        }
        str << (int)rowp[coli] << '\n';
        out->append(str.str());
        str.str(std::string());
        str.clear();
    }
}

void write_ppm(std::string filename, uint8_t **data, int width, int height) {
    std::ofstream outf;
    outf.open(filename, std::ofstream::out | std::ofstream::trunc);

    outf << "P3\n" << width << ' ' << height << "\n255\n";

    int part = (height + THREAD_CNT - 1) / THREAD_CNT;

    std::string buffers[THREAD_CNT * 2 - 1]; // align to 64 bytes
    std::thread bufferWriters[THREAD_CNT];

    bufferWriters[0] = std::thread(writeToBuffer, buffers, data, 0,
                                   std::min((part + 1) / 2, height), width);

    for (int i = 1; i < THREAD_CNT - 1; i++) {
        bufferWriters[i] =
            std::thread(writeToBuffer, buffers + (i << 1), data,
                        (i - 1) * part + (part + 1) / 2,
                        std::min(i * part + (part + 1) / 2, height), width);
    }

    bufferWriters[THREAD_CNT - 1] =
        std::thread(writeToBuffer, buffers + (THREAD_CNT * 2 - 2), data,
                    std::min((THREAD_CNT - 2) * part + (part + 1) / 2, height), height, width);

    for (int i = 0; i < THREAD_CNT; i++) {
        bufferWriters[i].join();
        outf << buffers[i << 1];
    }

    outf.close();
}

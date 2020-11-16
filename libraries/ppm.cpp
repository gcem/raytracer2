#include "ppm.h"
#include "constants.h"
#include <fstream>
#include <sstream>
#include <thread>

std::ofstream outf;

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

void write_ppm_header(std::string filename, int width, int height) {
    outf.open(filename, std::ofstream::out | std::ofstream::trunc);

    outf << "P3\n" << width << ' ' << height << "\n255\n";
}
#include "ppm.h"
#include <fstream>

void write_ppm(std::string filename, unsigned char* data, int width, int height)
{
    std::ofstream outf;
    outf.open(filename, std::ofstream::out | std::ofstream::trunc);

    outf << "P3\n" << width << ' ' << height << "\n255\n";
    for (int row = 0; row < height; row++) {
        int col = 0;
        for (; col < width * 3 - 1; col++) {
            outf << data[col] << ' ';
        }
        outf << data[col] << '\n';
    }

    outf.close();
}

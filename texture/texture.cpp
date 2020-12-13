#include "texture.h"
#include "jpeg.h"
#include <algorithm>
#include <cmath>

Texture::Texture(std::string imageName) {
    read_jpeg_header(imageName.c_str(), width, height);
    image = std::shared_ptr<unsigned char[]>(
        new unsigned char[width * height * 3]);
    read_jpeg(imageName.c_str(), image.get(), width, height);
}

Vec3f Texture::sample(float x, float y) {
    x *= width;
    y *= height;

    if (interpolation == Interpolation::Nearest) {
        int tx = round(x), ty = round(y);
        return pixelAt(tx, ty);
    } else {
        int xlo = x, xhi = ceil(x);
        int ylo = y, yhi = ceil(y);
        float dx = x - xlo;
        float dy = y - ylo;

        return pixelAt(xlo, ylo) * (1 - dx) * (1 - dy) +
               pixelAt(xlo, yhi) * (1 - dx) * dy +
               pixelAt(xhi, ylo) * dx * (1 - dy) +
               pixelAt(xhi, yhi) * dx * dy;
    }
}

void Texture::setInterpolation(Interpolation interp) {
    interpolation = interp;
}

void Texture::setWrap(Wrap wr) { wrap = wr; }

Vec3f Texture::pixelAt(int x, int y) {
    if (wrap == Wrap::Repeat) {
        x %= width;
        y %= height;
    } else {
        x = std::clamp(x, 0, width - 1);
        y = std::clamp(y, 0, height - 1);
    }

    int first = (y * width + x) * 3;

    return {(float)image.get()[first], (float)image.get()[first + 1],
            (float)image.get()[first + 2]};
}

#pragma once

#include "vector.h"
#include <memory>
#include <string>

class Texture {
public:
    enum class Interpolation {
        Nearest,
        Bilinear
    };

    enum class Wrap {
        Repeat,
        Clamp
    };

    enum class Mode {
        None,
        Diffuse,
        DiffuseBlend,
        TextureOnly
    };

    Texture() : image(nullptr), mode(Mode::None) {}
    Texture(std::string imageName);

    Vec3f sample(float x, float y);

    void setInterpolation(Interpolation);
    void setWrap(Wrap);

    // not handled in any function, for external use only
    Mode mode;

protected:
    Vec3f pixelAt(int x, int y);
    
    int width, height;
    std::shared_ptr<unsigned char[]> image;
    Interpolation interpolation;
    Wrap wrap;
};
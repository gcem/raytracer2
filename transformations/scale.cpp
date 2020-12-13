#include "scale.h"


Scale::Scale(float x, float y, float z) {
    matrix = Mat4({x, y, z, 1});
};
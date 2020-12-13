#include "rotate.h"

Rotate::Rotate(float angle, Vec3f axis) {
    float xangle = atan2(axis.y, axis.z);
    float yangle = -1 * atan2(axis.x, axis.z);

    matrix = rotateX(-xangle) * rotateY(-yangle) * rotateZ(angle) *
             rotateY(yangle) * rotateX(xangle);
}

Mat4 Rotate::rotateX(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    // clang-format off
    return {
        {1, 0,  0, 0},
        {0, c, -s, 0},
        {0, s,  c, 0},
        {0, 0,  0, 1}
    };
    // clang-format on

}

Mat4 Rotate::rotateY(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    // clang-format off
    return {
        { c, 0, s, 0},
        { 0, 1, 0, 0},
        {-s, 0, c, 0},
        { 0, 0, 0, 1}
    };
    // clang-format on
}

Mat4 Rotate::rotateZ(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    // clang-format off
    return {
        {c, -s, 0, 0},
        {s,  c, 0, 0},
        {0,  0, 1, 0},
        {0,  0, 0, 1}
    };
    // clang-format on
}

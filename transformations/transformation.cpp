#include "transformation.h"


Transformation::Transformation() : matrix() {
    
}

Transformation::Transformation(const Mat4 &mat) : matrix(mat) {
    
}

Transformation Transformation::operator*(const Transformation &other) {
    return Transformation(matrix * other.matrix);
}

Vec3f Transformation::operator()(const Vec3f &vec) {
    return matrix * Vec4f(vec, 1);
}

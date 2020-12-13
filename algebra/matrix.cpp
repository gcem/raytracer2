#include "matrix.h"

// clang-format off
Mat4::Mat4() : rows({
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }) { }

Mat4::Mat4(Vec4f d) : rows({
        {d.x, 0,     0, 0},
        {0,   d.y,   0, 0},
        {0,   0,   d.z, 0},
        {0,   0,     0, d.w}
    }) { }
// clang-format on

Mat4::Mat4(const Vec4f &row1, const Vec4f &row2, const Vec4f &row3,
           const Vec4f &row4)
    : rows({row1, row2, row3, row4}) {}

Mat4 Mat4::operator*(const Mat4 &other) const {
    Mat4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.rows[row][col] = rows[row].dot(other.column(col));
        }
    }
    return result;
}

Vec4f Mat4::operator*(const Vec4f &vec) const {
    Vec4f result;
    for (int row = 0; row < 4; row++) {
        result[row] = rows[row].dot(vec);
    }
    return result;
}

Mat4 Mat4::inverse() const {
    Mat4 inv = Mat4(); // start with identity
    Mat4 mat = *this;

    // make upper triangular
    for (int i = 0; i < 4; i++) {
        float first = mat[i][i];
        mat.rows[i] /= first;
        inv.rows[i] /= first;
        for (int row = i + 1; row < 4; row++) {
            float rowFirst = mat[row][i];
            mat.rows[row] -= mat.rows[i] * rowFirst;
            inv.rows[row] -= inv.rows[i] * rowFirst;
        }
    }

    // make identity
    for (int i = 3; i >= 0; i--) {
        for (int row = i - 1; row >= 0; row--) {
            // no need to update mat since we are going from right to left
            inv.rows[row] -= inv.rows[i] * mat[row][i];
        }
    }

    return inv;
}

Mat4 Mat4::transpose() const {
    return {column(0), column(1), column(2), column(3)};
}

const Vec4f &Mat4::operator[](int rowIndex) const { return rows[rowIndex]; }

Vec4f Mat4::column(int colIndex) const {
    return {rows[0][colIndex], rows[1][colIndex], rows[2][colIndex],
            rows[3][colIndex]};
}

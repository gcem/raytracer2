#pragma once

#include "vector.h"

// {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

class Mat4 {
public:
    // identity matrix
    Mat4();

    // diagonal matrix
    Mat4(Vec4f diag);

    Mat4(const Vec4f &row1, const Vec4f &row2, const Vec4f &row3,
         const Vec4f &row4);

    Mat4 operator*(const Mat4 &other) const;
    Vec4f operator*(const Vec4f &vec) const;

    // uses gaussian elimination
    Mat4 inverse() const; // realized after implementing, this is not used :(
    Mat4 transpose() const;

    // @param rowIndex zero-indexed
    const Vec4f &operator[](int rowIndex) const;

protected:
    Vec4f rows[4];

    Vec4f column(int colIndex) const;
};
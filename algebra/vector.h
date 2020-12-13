#pragma once

#include <cmath>

struct Vec3f {
    union {
        struct {
            float x, y, z;
        };
        float arr[3];
    };

    Vec3f operator*(float f) const { return Vec3f{x * f, y * f, z * f}; }

    Vec3f operator*(const Vec3f &v) const {
        return Vec3f{x * v.x, y * v.y, z * v.z};
    }

    Vec3f operator/(const Vec3f &v) const {
        return Vec3f{x / v.x, y / v.y, z / v.z};
    }

    Vec3f operator+(const Vec3f &v) const {
        return Vec3f{x + v.x, y + v.y, z + v.z};
    }

    Vec3f operator-(const Vec3f &v) const {
        return Vec3f{x - v.x, y - v.y, z - v.z};
    }

    Vec3f cross(const Vec3f &v) const {
        return Vec3f{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

    float dot(const Vec3f &v) const { return x * v.x + y * v.y + z * v.z; }
    // both vectors should be normalized
    float sine(const Vec3f &v) const {
        float d = dot(v);
        return sqrtf(1 - d * d);
    }

    Vec3f normalize() const {
        float len = sqrtf(x * x + y * y + z * z);
        return Vec3f{x / len, y / len, z / len};
        // if len == 0, inf?
    }

    float norm() const { return sqrtf(x * x + y * y + z * z); }

    void operator+=(const Vec3f &v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    void operator-=(const Vec3f &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    void operator/=(float f) {
        x /= f;
        y /= f;
        z /= f;
    }
    void operator*=(float f) {
        x *= f;
        y *= f;
        z *= f;
    }
};

struct Vec4f {
    union {
        struct {
            float x, y, z, w;
        };
        struct {
            float left, right, bottom, top;
        };
    };

    Vec4f() {}
    Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vec4f(Vec3f v, float last) : x(v.x), y(v.y), z(v.z), w(last) {}

    operator Vec3f() {
        return {x / w, y / w, z / w};
    }
};
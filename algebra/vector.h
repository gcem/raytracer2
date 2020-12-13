#pragma once

#include <cmath>

struct Vec3f {
    union {
        struct {
            float x, y, z;
        };
        float array[3];
    };

    float &operator[](int i) { return array[i]; }
    float operator[](int i) const { return array[i]; }

    float dot(const Vec3f &v) const { return x * v.x + y * v.y + z * v.z; }
    // both vectors should be normalized
    float sine(const Vec3f &v) const {
        float d = dot(v);
        return sqrtf(1 - d * d);
    }

    Vec3f cross(const Vec3f &v) const {
        return Vec3f{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

    Vec3f normalize() const {
        float len = sqrtf(x * x + y * y + z * z);
        return Vec3f{x / len, y / len, z / len};
        // if len == 0, inf?
    }

    float norm() const { return sqrtf(x * x + y * y + z * z); }

    Vec3f operator*(float f) const { return Vec3f{x * f, y * f, z * f}; }
    Vec3f operator/(float f) const { return Vec3f{x / f, y / f, z / f}; }

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
        float array[4];
    };

    Vec4f() {}
    Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vec4f(Vec3f v, float last) : x(v.x), y(v.y), z(v.z), w(last) {}

    operator Vec3f() { return {x / w, y / w, z / w}; }

    float &operator[](int i) { return array[i]; }
    float operator[](int i) const { return array[i]; }

    float dot(const Vec4f &v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    Vec4f operator*(float f) const { return Vec4f{x * f, y * f, z * f, w * f}; }

    Vec4f operator*(const Vec4f &v) const {
        return Vec4f{x * v.x, y * v.y, z * v.z, w * v.w};
    }

    Vec4f operator/(const Vec4f &v) const {
        return Vec4f{x / v.x, y / v.y, z / v.z, w / v.w};
    }

    Vec4f operator+(const Vec4f &v) const {
        return Vec4f{x + v.x, y + v.y, z + v.z, w + v.w};
    }

    Vec4f operator-(const Vec4f &v) const {
        return Vec4f{x - v.x, y - v.y, z - v.z, w - v.w};
    }

    void operator+=(const Vec4f &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
    }
    void operator-=(const Vec4f &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
    }
    void operator/=(float f) {
        x /= f;
        y /= f;
        z /= f;
        w /= f;
    }
    void operator*=(float f) {
        x *= f;
        y *= f;
        z *= f;
        w *= f;
    }
};

struct Vec2f {
    float x, y;

    Vec2f operator*(float f) const { return Vec2f{x * f, y * f}; }

    Vec2f operator*(const Vec2f &v) const {
        return Vec2f{x * v.x, y * v.y};
    }

    Vec2f operator/(const Vec2f &v) const {
        return Vec2f{x / v.x, y / v.y};
    }

    Vec2f operator+(const Vec2f &v) const {
        return Vec2f{x + v.x, y + v.y};
    }

    Vec2f operator-(const Vec2f &v) const {
        return Vec2f{x - v.x, y - v.y};
    }

    void operator+=(const Vec2f &v) {
        x += v.x;
        y += v.y;
    }
    void operator-=(const Vec2f &v) {
        x -= v.x;
        y -= v.y;
    }
    void operator/=(float f) {
        x /= f;
        y /= f;
    }
    void operator*=(float f) {
        x *= f;
        y *= f;
    }
};
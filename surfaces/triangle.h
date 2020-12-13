#pragma once

#include "surface.h"

class TriangleBase : public Surface {
public:
    struct Vertices {
        union {
            struct {
                Vec3f va, vb, vc;
            };
            Vec3f arr[3];
        };
        Vec3f &operator[](int i) { return arr[i]; }
        Vec3f operator[](int i) const { return arr[i]; }
    };

    TriangleBase() {}
    TriangleBase(const Vertices &v);

    // @returns -1 if no intersection, t otherwise
    float intersect(const Ray &ray, Ray &normalOut) override;

    bool intersectsBefore(const Ray &ray, float t) override;
    Vertices v;

protected:
    Vec3f normal, col1, col2;
    float area, detL;
};

class TexturedTriangleBase : public virtual TriangleBase {
public:
    struct TextureCoordinates {
        Vec2f va, vb, vc;
    };

    TexturedTriangleBase(const Vertices &v, const TextureCoordinates &t);

    float intersect(const Ray &ray, Ray &normalOut,
                    Vec2f &texCoordOut) override;

protected:
    TextureCoordinates texCoord;
};

class Triangle : public virtual TriangleBase {
public:
    Triangle(const Vertices &v, int materialId);

    int getMaterialId() const override;

protected:
    int materialId;
};

class TexturedTriangle : public virtual TexturedTriangleBase,
                         public virtual Triangle {
public:
    TexturedTriangle(const Vertices &v, int materialId, int textureId,
                     const TextureCoordinates &t);

    int getTextureId() const override { return textureId; }

protected:
    int textureId;
};
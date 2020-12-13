#pragma once

#include "box.h"
#include "surface.h"
#include "triangle.h"

class Mesh : public Surface {
public:
    Mesh(const std::vector<Triangle::Vertices> &faces, int materialId);

    Mesh(const std::vector<Triangle::Vertices> &faces, int materialId,
         int textureId,
         const std::vector<TexturedTriangle::TextureCoordinates> &texCoords);

    float intersect(const Ray &ray, Ray &normalOut) override;
    float intersect(const Ray &ray, Ray &normalOut,
                    Vec2f &texCoordOut) override;

    bool intersectsBefore(const Ray &ray, float t) override;

    int getMaterialId() const override { return materialId; }
    int getTextureId() const override { return textureId; }

protected:
    int materialId;
    int textureId = 0;
    BoundingBox box;
};
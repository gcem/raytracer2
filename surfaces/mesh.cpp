#include "mesh.h"
#include "constants.h"

Mesh::Mesh(const std::vector<TriangleBase::Vertices> &faces, int materialId)
    : materialId(materialId), box(faces) {}

Mesh::Mesh(const std::vector<Triangle::Vertices> &faces, int materialId,
           int textureId,
           const std::vector<TexturedTriangle::TextureCoordinates> &texCoords)
    : materialId(materialId), textureId(textureId), box(faces, texCoords) {}

float Mesh::intersect(const Ray &ray, Ray &normalOut) {
    return box.hit(ray, normalOut);
}

float Mesh::intersect(const Ray &ray, Ray &normalOut, Vec2f &texCoordOut) {
    return box.hit(ray, normalOut, texCoordOut);
}

bool Mesh::intersectsBefore(const Ray &ray, float t) {
    return box.intersectsBefore(ray, t);
}

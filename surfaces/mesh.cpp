#include "mesh.h"
#include "constants.h"

Mesh::Mesh(const std::vector<TriangleBase::Vertices> &faces, int materialId)
    : materialId(materialId), box(faces) {}

float Mesh::intersect(const Ray &ray, Ray &normalOut) {
    return box.hit(ray, normalOut);
}

int Mesh::getMaterialId() const { return materialId; }

bool Mesh::intersectsBefore(const Ray &ray, float t) {
    return box.intersectsBefore(ray, t);    
}

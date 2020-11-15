#include "mesh.h"
#include "constants.h"

Mesh::Mesh(const std::vector<TriangleBase::Vertices> &faces, int materialId)
    : materialId(materialId), box(faces) {}

float Mesh::intersect(const Ray &ray) { return box.hit(ray); }

void Mesh::normalAt(const Ray &ray, float t, Ray &out) {
    box.lastHit()->normalAt(ray, t, out);
}

int Mesh::getMaterialId() const { return materialId; }

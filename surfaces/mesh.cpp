#include "mesh.h"
#include "constants.h"

Mesh::Mesh(const std::vector<TriangleBase::Vertices> &faces, int materialId)
    : materialId(materialId), box(faces) {}

float Mesh::intersect(const Ray &ray) {
    t_meshMatId = materialId;
    return box.hit(ray);
}

int Mesh::getMaterialId() const { return materialId; }

#include "mesh.h"
#include "constants.h"

Mesh::Mesh(std::vector<TriangleBase::Vertices> faces, int materialId)
    : materialId(materialId) {
    triangles.resize(faces.size());
    for (int i = 0; i < faces.size(); i++) {
        triangles[i] = TriangleBase(faces[i]);
    }
}

float Mesh::intersect(const Ray &ray) {
    float minT = MAXFLOAT / 2;
    TriangleBase *closest = nullptr;
    for (auto &triangle : triangles) {
        float t = triangle.intersect(ray);
        if (t > 0 && t < minT) {
            minT = t;
            closest = &triangle;
        }
    }
    if (closest) {
        closest->normalAt(ray, minT, lastNormal);
        return minT;
    }
    return -1;
}

void Mesh::normalAt(const Ray &ray, float t, Ray &out) {
    out = lastNormal;
}

void Mesh::addFace(Triangle::Vertices vertices) {
    triangles.push_back(TriangleBase(vertices));    
}

int Mesh::getMaterialId() const {
    return materialId;    
}

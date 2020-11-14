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
    if (!bruteForce) {
        // check the bounding box
        Vec3f mint = (box.min - ray.origin) / ray.direction;
        Vec3f maxt = (box.max - ray.origin) / ray.direction;
        if (mint.x > maxt.x)
            std::swap(mint.x, maxt.x);
        if (mint.y > maxt.y)
            std::swap(mint.y, maxt.y);
        if (mint.z > maxt.z)
            std::swap(mint.z, maxt.z);

        float tlow = std::max(mint.x, mint.y);
        tlow = std::max(tlow, mint.z);
        float thigh = std::min(maxt.x, maxt.y);
        thigh = std::min(thigh, maxt.z);

        if (thigh - tlow <
            -0.001) // allow some error for boxes with no volume
            return -1;
    }

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

void Mesh::normalAt(const Ray &ray, float t, Ray &out) { out = lastNormal; }

void Mesh::addFace(Triangle::Vertices vertices) {
    triangles.push_back(TriangleBase(vertices));
}

void Mesh::updateFaces() {
    if (triangles.size() <= 3) {
        bruteForce = true;
        return;
    }
    box.max.x = box.max.y = box.max.z = MINFLOAT;
    box.min.x = box.min.y = box.min.z = MAXFLOAT;
    for (auto &triangle : triangles) {
        auto &va = triangle.v.va, &vb = triangle.v.vb, &vc = triangle.v.vc;
        if (va.x < box.min.x)
            box.min.x = va.x;
        if (va.x > box.max.x)
            box.max.x = va.x;
        if (va.y < box.min.y)
            box.min.y = va.y;
        if (va.y > box.max.y)
            box.max.y = va.y;
        if (va.z < box.min.z)
            box.min.z = va.z;
        if (va.z > box.max.z)
            box.max.z = va.z;

        if (vb.x < box.min.x)
            box.min.x = vb.x;
        if (vb.x > box.max.x)
            box.max.x = vb.x;
        if (vb.y < box.min.y)
            box.min.y = vb.y;
        if (vb.y > box.max.y)
            box.max.y = vb.y;
        if (vb.z < box.min.z)
            box.min.z = vb.z;
        if (vb.z > box.max.z)
            box.max.z = vb.z;

        if (vc.x < box.min.x)
            box.min.x = vc.x;
        if (vc.x > box.max.x)
            box.max.x = vc.x;
        if (vc.y < box.min.y)
            box.min.y = vc.y;
        if (vc.y > box.max.y)
            box.max.y = vc.y;
        if (vc.z < box.min.z)
            box.min.z = vc.z;
        if (vc.z > box.max.z)
            box.max.z = vc.z;
    }
}

int Mesh::getMaterialId() const { return materialId; }

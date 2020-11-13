#include "triangle.h"
#include "constants.h"

TriangleBase::TriangleBase(const Vertices &v) : v(v) {
    normal = (v.vb - v.va).cross(v.vc - v.vb);
    area = normal.norm() / 2;
    normal = normal.normalize();

    col1 = v.va - v.vb;
    col2 = v.va - v.vc;
    detL = col1.y * col2.z - col1.z * col2.y;
}

Triangle::Triangle(const Vertices &v, int materialId)
    : TriangleBase(v), materialId(materialId) {}

float TriangleBase::intersect(const Ray &ray) {
    const Vec3f &col3 = ray.direction;

    Vec3f ve = v.va - ray.origin;

    // determinants of 2 by 2 matrices
    // .. detL = col1.y * col2.z - col1.z * col2.y;  in constructor
    float detR = col2.y * col3.z - col2.z * col3.y;
    float detM = col1.y * col3.z - col1.z * col3.y;

    float detA = col1.x * detR - col2.x * detM + col3.x * detL;

    float detBeta = ve.x * detR - col2.x * (ve.y * col3.z - ve.z * col3.y) +
                    col3.x * (ve.y * col2.z - ve.z * col2.y);
    float beta = detBeta / detA;
    if (beta < 0 || beta > 1)
        return -1;

    float detGamma = col1.x * (ve.y * col3.z - ve.z * col3.y) - ve.x * detM +
                     col3.z * (col1.y * ve.z - col1.z * ve.y);
    float gamma = detGamma / detA;
    if (gamma < 0 || beta + gamma > 1)
        return -1;

    float detT = col1.x * (col2.y * ve.z - col2.z * ve.y) -
                 col2.x * (col1.y * ve.z - col1.z * ve.y) + ve.x * detL;
    float t = detT / detA;

    if (t < 0.0001)
        return -1;

    return t;
}

void TriangleBase::normalAt(const Ray &ray, float t, Ray &out) {
    out.origin = ray.origin + ray.direction * t;
    out.direction = normal;
}

int Triangle::getMaterialId() const {
    return materialId;    
}

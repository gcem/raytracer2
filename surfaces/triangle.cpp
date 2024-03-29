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

float TriangleBase::intersect(const Ray &ray, Ray &normalOut) {
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
    if (beta < -TRIANGLE_EPSILON || beta > 1)
        return -1;

    float detGamma = col1.x * (ve.y * col3.z - ve.z * col3.y) - ve.x * detM +
                     col3.x * (col1.y * ve.z - col1.z * ve.y);
    float gamma = detGamma / detA;
    if (gamma < -TRIANGLE_EPSILON || beta + gamma > 1)
        return -1;

    float detT = col1.x * (col2.y * ve.z - col2.z * ve.y) -
                 col2.x * (col1.y * ve.z - col1.z * ve.y) + ve.x * detL;
    float t = detT / detA;

    if (t < TRIANGLE_EPSILON)
        return -1;

    normalOut.origin = ray.origin + ray.direction * t;
    if (ray.direction.dot(normal) < 0)
        normalOut.direction = normal;
    else
        normalOut.direction = normal * -1;

    return t;
}

int Triangle::getMaterialId() const { return materialId; }

bool TriangleBase::intersectsBefore(const Ray &ray, float maxt) {
    const Vec3f &col3 = ray.direction;

    Vec3f ve = v.va - ray.origin;

    // determinants of 2 by 2 matrices
    // .. detL = col1.y * col2.z - col1.z * col2.y;  in constructor
    float detR = col2.y * col3.z - col2.z * col3.y;
    float detM = col1.y * col3.z - col1.z * col3.y;

    float detA = col1.x * detR - col2.x * detM + col3.x * detL;

    float detT = col1.x * (col2.y * ve.z - col2.z * ve.y) -
                 col2.x * (col1.y * ve.z - col1.z * ve.y) + ve.x * detL;
    float t = detT / detA;
    if (t < TRIANGLE_EPSILON || t > maxt)
        return false;

    float detBeta = ve.x * detR - col2.x * (ve.y * col3.z - ve.z * col3.y) +
                    col3.x * (ve.y * col2.z - ve.z * col2.y);
    float beta = detBeta / detA;
    if (beta < -TRIANGLE_EPSILON || beta > 1)
        return false;

    float detGamma = col1.x * (ve.y * col3.z - ve.z * col3.y) - ve.x * detM +
                     col3.x * (col1.y * ve.z - col1.z * ve.y);
    float gamma = detGamma / detA;
    if (gamma < -TRIANGLE_EPSILON || beta + gamma > 1)
        return false;

    return true;
}

TexturedTriangleBase::TexturedTriangleBase(const Vertices &v,
                                           const TextureCoordinates &t)
    : TriangleBase(v), texCoord(t) {}

TexturedTriangle::TexturedTriangle(const Vertices &v, int materialId,
                                   int textureId, const TextureCoordinates &t)
    : TriangleBase(v), TexturedTriangleBase(v, t), Triangle(v, materialId), textureId(textureId) {}

float TexturedTriangleBase::intersect(const Ray &ray, Ray &normalOut,
                                      Vec2f &texCoordOut) {
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
    if (beta < -TRIANGLE_EPSILON || beta > 1 + TRIANGLE_EPSILON)
        return -1;

    float detGamma = col1.x * (ve.y * col3.z - ve.z * col3.y) - ve.x * detM +
                     col3.x * (col1.y * ve.z - col1.z * ve.y);
    float gamma = detGamma / detA;
    float alpha = 1 - gamma - beta;
    if (gamma < -TRIANGLE_EPSILON || alpha < -TRIANGLE_EPSILON)
        return -1;

    float detT = col1.x * (col2.y * ve.z - col2.z * ve.y) -
                 col2.x * (col1.y * ve.z - col1.z * ve.y) + ve.x * detL;
    float t = detT / detA;

    if (t < TRIANGLE_EPSILON)
        return -1;

    normalOut.origin = ray.origin + ray.direction * t;
    if (ray.direction.dot(normal) < 0)
        normalOut.direction = normal;
    else
        normalOut.direction = normal * -1;

    texCoordOut =
        texCoord.va * alpha + texCoord.vb * beta + texCoord.vc * gamma;

    return t;
}
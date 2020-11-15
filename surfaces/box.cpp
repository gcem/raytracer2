#include "box.h"
#include "constants.h"
#include <thread>

BoundingBox::BoundingBox(const std::vector<Triangle::Vertices> &vertices) {
    hasBox = false;
    hasChildren = false;

    if (vertices.size() <= BRUTEFORCE_THRESHOLD) {
        for (auto &tri : vertices)
            triangles.push_back(TriangleBase(tri));
        return;
    }

    hasBox = true;

    max.x = max.y = max.z = MINFLOAT_CONST;
    min.x = min.y = min.z = MAXFLOAT_CONST;
    for (auto &v : vertices) {
        min.x = std::min(min.x, v.va.x);
        min.y = std::min(min.y, v.va.y);
        min.z = std::min(min.z, v.va.z);
        max.x = std::max(max.x, v.va.x);
        max.y = std::max(max.y, v.va.y);
        max.z = std::max(max.z, v.va.z);

        min.x = std::min(min.x, v.vb.x);
        min.y = std::min(min.y, v.vb.y);
        min.z = std::min(min.z, v.vb.z);
        max.x = std::max(max.x, v.vb.x);
        max.y = std::max(max.y, v.vb.y);
        max.z = std::max(max.z, v.vb.z);

        min.x = std::min(min.x, v.vc.x);
        min.y = std::min(min.y, v.vc.y);
        min.z = std::min(min.z, v.vc.z);
        max.x = std::max(max.x, v.vc.x);
        max.y = std::max(max.y, v.vc.y);
        max.z = std::max(max.z, v.vc.z);
    }

    if (vertices.size() <= SUBDIVISION_THRESHOLD) {
        for (auto &tri : vertices)
            triangles.push_back(TriangleBase(tri));
        return;
    }

    // divide to left and right boxes
    int divisionAxis = chooseAxis();
    for (int i = 0; i < 3; i++) {
        divisionAxis = (divisionAxis + i) % 3;

        float middle = (min.arr[divisionAxis] + max.arr[divisionAxis]) / 2;
        std::vector<Triangle::Vertices> leftvert, rightvert;
        for (auto &v : vertices) {
            bool toLeft = false, toRight = false;
            for (int vi = 0; vi < 3; vi++) {
                if (v.arr[vi].arr[divisionAxis] <= middle)
                    toLeft = true;
                if (v.arr[vi].arr[divisionAxis] >= middle) // equal: both
                    toRight = true;
            }
            if (toLeft)
                leftvert.push_back(v);
            if (toRight)
                rightvert.push_back(v);
        }

        if (leftvert.size() == vertices.size() ||
            rightvert.size() == vertices.size()) // this axis failed
            continue;

        hasChildren = true;

        if (leftvert.size() >= BOX_MULTITHREAD_THRESHOLD &&
            rightvert.size() >= BOX_MULTITHREAD_THRESHOLD) {
            std::thread leftThread(
                [&]() { left = new BoundingBox(leftvert); });
            std::thread rightThread(
                [&]() { right = new BoundingBox(rightvert); });
            leftThread.join();
            rightThread.join();
        } else {
            left = new BoundingBox(leftvert);
            right = new BoundingBox(rightvert);
        }
        return;
    }

    // no suitable division. this is a leaf now
    for (auto &tri : vertices)
        triangles.push_back(TriangleBase(tri));
    return;
}

float BoundingBox::hit(const Ray &ray) {
    if (hasBox) {
        if (boxT(ray) >= t_minT) // miss or far
            return -1;
    }

    // ray hit the box

    if (hasChildren) {
        // TODO: how do we return the normal???

        if (left->boxT(ray) < right->boxT(ray)) {
            left->hit(ray);
            right->hit(ray);
        } else {
            right->hit(ray);
            left->hit(ray);
        }
        return 0; // TODO: clean
    }

    // we have triangles

    for (auto &triangle : triangles) {
        triangle.intersect(ray);
    }
}

BoundingBox::Axis BoundingBox::chooseAxis() {
    float dx = max.x - min.x, dy = max.y - min.y, dz = max.z - min.z;
    if (dx > dy && dx > dz)
        return Axis::x;
    if (dy > dz)
        return Axis::y;
    return Axis::z;
}

float BoundingBox::boxT(const Ray &ray) {
    // check the bounding box
    Vec3f mint = (min - ray.origin) / ray.direction;
    Vec3f maxt = (max - ray.origin) / ray.direction;
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

    if (thigh - tlow < -0.001 ||
        thigh < 0) // allow some error for boxes with no volume
        return MAXFLOAT_CONST;
    return tlow;
}

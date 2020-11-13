#include "ray.h"

class Surface {
public:
    // @returns -1 if no intersection, t otherwise
    virtual float intersect(const Ray &ray) = 0;

    // mesh will store the normal for the last call to intersect()
    // and return this normal
    virtual void normalAt(const Ray &ray, float t, Ray &out) = 0;

protected:
    Surface() {}
};
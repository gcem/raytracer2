#include "parser.h"

struct Ray {
    Vec3f origin;
    Vec3f direction;
};

void castRays(std::vector<Ray> &out, const Camera &camera);
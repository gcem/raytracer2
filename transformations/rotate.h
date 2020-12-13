#include "transformation.h"

class Rotate : public Transformation {
public:
    // right-handed rotation of angle radians about axis
    Rotate(float angle, Vec3f axis);

protected:
    Mat4 rotateX(float angle);
    Mat4 rotateY(float angle);
    Mat4 rotateZ(float angle);
};
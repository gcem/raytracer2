#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include "ray.h"
#include "sphere.h"
#include "triangle.h"
#include "mesh.h"
#include "camera.h"
#include "vector.h"
#include "texture.h"
#include "transformation.h"

#include <cmath>
#include <string>
#include <vector>

// Notice that all the structures are as simple as possible
// so that you are not enforced to adopt any style or design.

struct PointLight {
    Vec3f position;
    Vec3f intensity;
};

struct Material {
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    Vec3f mirror;
    float phong_exponent;
};

struct Scene {
    // Data
    Vec3f background_color;
    float shadow_ray_epsilon;
    int max_recursion_depth;
    std::vector<Camera> cameras;
    Vec3f ambient_light;
    std::vector<PointLight> point_lights;
    std::vector<Material> materials;
    std::vector<Vec3f> vertex_data;
    std::vector<Surface*> surfaces;
    std::vector<Texture> textures;

    // Functions
    void loadFromXml(const std::string &filepath);
};

#endif
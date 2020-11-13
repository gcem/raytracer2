#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include "ray.h"
#include "sphere.h"
#include "triangle.h"
#include "mesh.h"
#include "vector.h"

#include <cmath>
#include <string>
#include <vector>

// Notice that all the structures are as simple as possible
// so that you are not enforced to adopt any style or design.

struct Camera {
    Vec3f position;
    Vec3f gaze;
    Vec3f up;
    Vec4f near_plane;
    float near_distance;
    int width, height;
    std::string image_name;
};

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
    Vec3i background_color;
    float shadow_ray_epsilon;
    int max_recursion_depth;
    std::vector<Camera> cameras;
    Vec3f ambient_light;
    std::vector<PointLight> point_lights;
    std::vector<Material> materials;
    std::vector<Vec3f> vertex_data;
    std::vector<Mesh> meshes;
    std::vector<Triangle> triangles;
    std::vector<Sphere> spheres;

    // Functions
    void loadFromXml(const std::string &filepath);
};

#endif
#include "parser.h"
#include "rotate.h"
#include "scale.h"
#include "tinyxml2.h"
#include "translate.h"

#include <sstream>
#include <stdexcept>
#include <thread>

void Scene::loadFromXml(const std::string &filepath) {
    tinyxml2::XMLDocument file;
    std::stringstream stream;

    auto res = file.LoadFile(filepath.c_str());
    if (res) {
        throw std::runtime_error("Error: The xml file cannot be loaded.");
    }

    auto root = file.FirstChild();
    if (!root) {
        throw std::runtime_error("Error: Root is not found.");
    }

    // Get BackgroundColor
    auto element = root->FirstChildElement("BackgroundColor");
    if (element) {
        stream << element->GetText() << std::endl;
    } else {
        stream << "0 0 0" << std::endl;
    }
    stream >> background_color.x >> background_color.y >> background_color.z;

    // Get ShadowRayEpsilon
    element = root->FirstChildElement("ShadowRayEpsilon");
    if (element) {
        stream << element->GetText() << std::endl;
    } else {
        stream << "0.001" << std::endl;
    }
    stream >> shadow_ray_epsilon;

    // Get MaxRecursionDepth
    element = root->FirstChildElement("MaxRecursionDepth");
    if (element) {
        stream << element->GetText() << std::endl;
    } else {
        stream << "0" << std::endl;
    }
    stream >> max_recursion_depth;

    // Get Cameras
    element = root->FirstChildElement("Cameras");
    element = element->FirstChildElement("Camera");
    Camera camera;
    while (element) {
        auto child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Gaze");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Up");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearPlane");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearDistance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageResolution");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageName");
        stream << child->GetText() << std::endl;

        stream >> camera.position.x >> camera.position.y >> camera.position.z;
        stream >> camera.gaze.x >> camera.gaze.y >> camera.gaze.z;
        stream >> camera.up.x >> camera.up.y >> camera.up.z;
        stream >> camera.near_plane.x >> camera.near_plane.y >>
            camera.near_plane.z >> camera.near_plane.w;
        stream >> camera.near_distance;
        stream >> camera.width >> camera.height;
        stream >> camera.image_name;
        camera.image_name = "output/" + camera.image_name;

        cameras.push_back(camera);
        element = element->NextSiblingElement("Camera");
    }

    // Get Lights
    element = root->FirstChildElement("Lights");
    auto child = element->FirstChildElement("AmbientLight");
    stream << child->GetText() << std::endl;
    stream >> ambient_light.x >> ambient_light.y >> ambient_light.z;
    element = element->FirstChildElement("PointLight");
    PointLight point_light;
    while (element) {
        child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Intensity");
        stream << child->GetText() << std::endl;

        stream >> point_light.position.x >> point_light.position.y >>
            point_light.position.z;
        stream >> point_light.intensity.x >> point_light.intensity.y >>
            point_light.intensity.z;

        point_lights.push_back(point_light);
        element = element->NextSiblingElement("PointLight");
    }

    int id;

    // Get Materials
    materials.push_back({}); // make 1-indexed
    element = root->FirstChildElement("Materials");
    element = element->FirstChildElement("Material");
    Material material;
    while (element) {
        child = element->FirstChildElement("AmbientReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("DiffuseReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("SpecularReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("PhongExponent");
        stream << child->GetText() << std::endl;

        stream >> material.ambient.x >> material.ambient.y >>
            material.ambient.z;
        stream >> material.diffuse.x >> material.diffuse.y >>
            material.diffuse.z;
        stream >> material.specular.x >> material.specular.y >>
            material.specular.z;
        stream >> material.phong_exponent;

        child = element->FirstChildElement("MirrorReflectance");
        if (child) {
            stream << child->GetText() << std::endl;
            stream >> material.mirror.x >> material.mirror.y >>
                material.mirror.z;
        } else
            material.mirror = {0, 0, 0};

        materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }

    // Get VertexData
    std::vector<Vec3f> vertex_data(
        1); // for padding - vertex ids are 1-indexed
    element = root->FirstChildElement("VertexData");
    stream << element->GetText() << std::endl;
    Vec3f vertex;
    while (!(stream >> vertex.x).eof()) {
        stream >> vertex.y >> vertex.z;
        vertex_data.push_back(vertex);
    }
    stream.clear();

    // Get TexCoordData
    std::vector<Vec2f> texture_coordinates(1);
    element = root->FirstChildElement("TexCoordData");

    if (element) {
        stream << element->GetText() << std::endl;
        Vec2f tex_coord;
        while (!(stream >> tex_coord.x).eof()) {
            stream >> tex_coord.y;
            texture_coordinates.push_back(tex_coord);
        }

        stream.clear();
    }

    int materialId;
    int vid1, vid2, vid3;

    // Get Textures
    textures = std::vector<Texture>(1);
    element = root->FirstChildElement("Textures");

    if (element) {
        element = element->FirstChildElement("Texture");

        while (element) {
            child = element->FirstChildElement("ImageName");
            textures.push_back(Texture(child->GetText()));

            std::string text;

            child = element->FirstChildElement("Interpolation");
            text = child->GetText();
            if (text.compare("nearest") == 0) {
                textures.back().setInterpolation(
                    Texture::Interpolation::Nearest);
            } else {
                textures.back().setInterpolation(
                    Texture::Interpolation::Bilinear);
            }

            child = element->FirstChildElement("Appearance");
            text = child->GetText();
            if (text.compare("clamp") == 0) {
                textures.back().setWrap(Texture::Wrap::Clamp);
            } else {
                textures.back().setWrap(Texture::Wrap::Repeat);
            }

            child = element->FirstChildElement("DecalMode");
            text = child->GetText();
            if (text.compare("replace_kd") == 0) {
                textures.back().mode = Texture::Mode::Diffuse;
            } else if (text.compare("blend_kd") == 0) {
                textures.back().mode = Texture::Mode::DiffuseBlend;
            } else {
                textures.back().mode = Texture::Mode::TextureOnly;
            }
            element = element->NextSiblingElement("Texture");
        }
    }

    // Get Transformations
    std::vector<Translate> translate(1); // make one-indexed
    std::vector<Scale> scale(1);
    std::vector<Rotate> rotate(1);

    element = root->FirstChildElement("Transformations");

    if (element) {
        child = element->FirstChildElement();

        while (child) {
            const char *tagName = child->Name();

            if (!strcmp(tagName, "Translation")) {
                float dx, dy, dz;
                stream << child->GetText() << std::endl;
                stream >> dx >> dy >> dz;

                translate.push_back(Translate(dx, dy, dz));
            } else if (!strcmp(tagName, "Scaling")) {
                float sx, sy, sz;
                stream << child->GetText() << std::endl;
                stream >> sx >> sy >> sz;

                scale.push_back(Scale(sx, sy, sz));
            } else if (!strcmp(tagName, "Rotation")) {
                float angle, x, y, z;

                stream << child->GetText() << std::endl;
                stream >> angle >> x >> y >> z;

                rotate.push_back(Rotate(angle * M_PI / 180, {x, y, z}));
            }

            child = child->NextSiblingElement();
        }
    }

    // Get Meshes
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Mesh");
    Mesh *mesh;
    std::vector<std::thread> meshThreads;
    while (element) {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> materialId;

        // transformations
        Transformation tfm;
        child = element->FirstChildElement("Transformations");

        if (child) {
            stream << child->GetText();
            char c = ' ';
            int i;
            while (!stream.eof()) {
                switch (c) {
                case 't':
                    stream >> i;
                    tfm = translate[i] * tfm;
                    break;
                case 'r':
                    stream >> i;
                    tfm = rotate[i] * tfm;
                    break;
                case 's':
                    stream >> i;
                    tfm = scale[i] * tfm;
                    break;
                }
                stream >> c;
            }
            stream.clear(); // reset EOF flag
        }

        int textureId = 0;
        child = element->FirstChildElement("Texture");

        if (child) {
            stream << child->GetText() << std::endl;
            stream >> textureId;
        }

        child = element->FirstChildElement("Faces");
        stream << child->GetText() << std::endl;
        auto *vertices = new std::vector<TriangleBase::Vertices>();
        auto *texCoords =
            new std::vector<TexturedTriangleBase::TextureCoordinates>();
        if (textureId)
            while (!(stream >> vid1).eof()) {
                stream >> vid2 >> vid3;
                vertices->push_back({tfm(vertex_data[vid1]),
                                     tfm(vertex_data[vid2]),
                                     tfm(vertex_data[vid3])});
                texCoords->push_back({texture_coordinates[vid1],
                                      texture_coordinates[vid2],
                                      texture_coordinates[vid3]});
            }
        else
            while (!(stream >> vid1).eof()) {
                stream >> vid2 >> vid3;
                vertices->push_back({tfm(vertex_data[vid1]),
                                     tfm(vertex_data[vid2]),
                                     tfm(vertex_data[vid3])});
            }
        surfaces.push_back(nullptr);
        meshThreads.push_back(std::thread(
            [](std::vector<TriangleBase::Vertices> *vertices, int materialId,
               int textureId,
               std::vector<TexturedTriangleBase::TextureCoordinates>
                   *texCoords,
               std::vector<Surface *> *surfaces, int surfaceIndex) {
                if (textureId)
                    (*surfaces)[surfaceIndex] = new Mesh(
                        *vertices, materialId, textureId, *texCoords);
                else
                    (*surfaces)[surfaceIndex] =
                        new Mesh(*vertices, materialId);
                delete vertices;
            },
            vertices, materialId, textureId, texCoords, &surfaces,
            (int)(surfaces.size() - 1)));
        stream.clear();
        element = element->NextSiblingElement("Mesh");
    }
    stream.clear();

    // Get Triangles
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Triangle");

    while (element) {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> materialId;

        // transformations
        Transformation tfm;
        child = element->FirstChildElement("Transformations");

        if (child) {
            stream << child->GetText();
            char c = ' ';
            int i;
            while (!stream.eof()) {
                switch (c) {
                case 't':
                    stream >> i;
                    tfm = translate[i] * tfm;
                    break;
                case 'r':
                    stream >> i;
                    tfm = rotate[i] * tfm;
                    break;
                case 's':
                    stream >> i;
                    tfm = scale[i] * tfm;
                    break;
                }
                stream >> c;
            }
            stream.clear(); // reset EOF flag
        }

        child = element->FirstChildElement("Indices");
        stream << child->GetText() << std::endl;
        stream >> vid1 >> vid2 >> vid3;

        child = element->FirstChildElement("Texture");

        if (child) {
            int texId;
            stream << child->GetText() << std::endl;
            stream >> texId;
            surfaces.push_back(new TexturedTriangle(
                {tfm(vertex_data[vid1]), tfm(vertex_data[vid2]),
                 tfm(vertex_data[vid3])},
                materialId, texId,
                {texture_coordinates[vid1], texture_coordinates[vid2],
                 texture_coordinates[vid3]}));
        } else {
            surfaces.push_back(
                new Triangle({tfm(vertex_data[vid1]), tfm(vertex_data[vid2]),
                              tfm(vertex_data[vid3])},
                             materialId));
        }
        element = element->NextSiblingElement("Triangle");
    }

    // Get Spheres
    float radius;

    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Sphere");
    while (element) {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> materialId;

        child = element->FirstChildElement("Center");
        stream << child->GetText() << std::endl;
        stream >> vid1; // center

        child = element->FirstChildElement("Radius");
        stream << child->GetText() << std::endl;
        stream >> radius;

        int texId = 0;
        child = element->FirstChildElement("Texture");
        Vec3f up, right;
        if (child) {
            stream << child->GetText() << std::endl;
            stream >> texId;
            up = {0, 1, 0};
            right = {1, 0, 0};
        }
        // sphere-specific transformations
        Transformation tfm;
        child = element->FirstChildElement("Transformations");

        if (child) {
            stream << child->GetText();
            char c = ' ';
            int i;
            while (!stream.eof()) {
                switch (c) {
                case 't':
                    stream >> i;
                    tfm = translate[i] * tfm;
                    break;
                case 'r':
                    stream >> i;
                    tfm = rotate[i] * tfm;
                    if (texId) {
                        up = rotate[i](up);
                        right = rotate[i](right);
                    }
                    break;
                case 's':
                    stream >> i;
                    tfm = scale[i] * tfm;
                    radius *= scale[i].amount();
                    break;
                }
                stream >> c;
            }
            stream.clear(); // reset EOF flag
        }
        if (texId)
            surfaces.push_back(
                new TexturedSphere(tfm(vertex_data[vid1]), radius, materialId,
                                   texId, up, right));
        else
            surfaces.push_back(
                new Sphere(tfm(vertex_data[vid1]), radius, materialId));
        element = element->NextSiblingElement("Sphere");
    }

    for (auto &thread : meshThreads)
        thread.join();
}
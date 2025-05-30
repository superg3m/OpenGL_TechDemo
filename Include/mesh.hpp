#pragma once

#include <gm.h>
#include <shader.hpp>
#include <vector>

struct Vertex {
    GM_Vec3 position;
    GM_Vec3 color;
    GM_Vec3 normal;
    GM_Vec2 uv;
};

struct Geometry {
    unsigned int VBO, VAO, EBO;
    unsigned int stride;
    std::vector<unsigned int> attributes;
    std::vector<float> vertices;
    std::vector<unsigned int> indicies;

    Geometry() = default;
    Geometry(std::vector<unsigned int> attributes, std::vector<float>& vertices, std::vector<unsigned int> indicies = std::vector<unsigned int>());
    static Geometry Cube(std::vector<unsigned int> attributes = {3, 0, 3, 2});
    static Geometry Sphere(int segments, std::vector<unsigned int> attributes = {3, 0, 3, 2});
    static Geometry Torus(std::vector<unsigned int> attributes = {3, 0, 3, 2});

private:
    void setup();
};

typedef int GLTextureID;
struct Material {
    Shader* shader;
    GM_RGB color;
    GLTextureID textures[TEXTURE_MAX];
    GLTextureID alphaMap;
    GLTextureID heightMap;
    GLTextureID normalMap;

    Material() = default;
    Material(Shader* shader);
    static Material BasicMaterial();    // Just basic color no lighting
    static Material StandardMaterial(); // Physics Based Rendering (PBR)
};

struct Mesh {
    Geometry geometry;
    Material material;
    GM_Matrix4 transform;

    Mesh() = default;
    Mesh(Material material, Geometry geometry, GM_Matrix4 transform = gm_mat4_identity());
    void draw();
};
#pragma once

#include <glad/glad.h>
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
    unsigned int VBO = 0, VAO = 0, EBO = 0;
    unsigned int stride = 0;
    std::vector<unsigned int> attributes;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    GLenum usage = GL_STATIC_DRAW; // Renamed from binded_draw_state for clarity

    Geometry() = default;
    Geometry(const std::vector<unsigned int>& attributes, 
             const std::vector<float>& vertices,
             const std::vector<unsigned int>& indices = {},
             GLenum usage = GL_STATIC_DRAW);

    // Factory methods for common shapes
    static Geometry Cube(const std::vector<unsigned int>& attributes = {3, 0, 3, 2});
    static Geometry Sphere(int segments, const std::vector<unsigned int>& attributes = {3, 0, 3, 2});
    static Geometry Torus(const std::vector<unsigned int>& attributes = {3, 0, 3, 2});

private:
    void setup();
    friend struct Mesh; // Allow Mesh to call setup if needed
};

using GLTextureID = int;

struct Material {
    Shader* shader = nullptr;
    GM_RGBA color;
    GLTextureID textures[TEXTURE_MAX] = {0};
    GLTextureID alphaMap = 0;
    GLTextureID heightMap = 0;
    GLTextureID normalMap = 0;

    Material() = default;
    explicit Material(Shader* shader);

    static Material BasicMaterial();    // Simple color, no lighting
    static Material StandardMaterial(); // PBR (Physics Based Rendering)
};

// TODO(Jovanni): Consider removing transform from Mesh, 
// as entities typically have transforms, not meshes themselves.
struct Mesh {
    Geometry geometry;
    Material material;
    GM_Matrix4 transform = gm_mat4_identity();
    GLenum draw_type = GL_TRIANGLES; // GL_TRIANGLES, GL_LINES, ...

    Mesh() = default;
    Mesh(Material material, Geometry geometry, GM_Matrix4 transform = gm_mat4_identity(), GLenum draw_type = GL_TRIANGLES);

    void draw();
    void setVertices(const std::vector<float>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
};
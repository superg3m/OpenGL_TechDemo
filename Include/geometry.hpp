#pragma once

#include <gm.h>
#include <glad/glad.h>
#include <vector>

struct Vertex {
    GM_Vec3 position;
    GM_Vec3 normal;
    GM_Vec2 uv;
};

struct Geometry {
    unsigned int VBO = 0, VAO = 0, EBO = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    GLenum usage;

    Geometry() = default;

    /**
     * @brief vertices must be in 
     * 
     * @param vertices 
     * @param indices 
     * @param usage 
     */
    Geometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, GLenum usage = GL_STATIC_DRAW);

    static Geometry Quad();
    static Geometry Cube();
    static Geometry Sphere(int segments);

    void setup();
};
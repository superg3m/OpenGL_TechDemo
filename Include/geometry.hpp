#pragma once

#include <vector>

#include <gm.hpp>
#include <Vertex.hpp>

#include <glad/glad.h>

struct Geometry {
    // Date: June 15, 2025
    // TODO(Jovanni): I should move out the VAO and stuff becuae I should be able to cache the mesh vertex data just using the VAO as a "GeometryID"
    // The tricky part here is figuring out to request this Geometry I think its just gonna be another Key lookup
    unsigned int VBO = 0, VAO = 0, EBO = 0;
    unsigned int vertex_count;
    unsigned int index_count;
    GLenum draw_type; // GL_TRIANGLES, GL_LINES, ...

    Geometry();

    /**
     * @brief vertices must be in 
     * 
     * @param vertices 
     * @param indices 
     * @param usage 
     */
    Geometry(VertexAttributeFlag flags, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, GLenum draw_type = GL_TRIANGLES);

    static Geometry Quad();
    static Geometry AABB();
    static Geometry Cube();
    static Geometry Sphere(int segments);

    void setup(VertexAttributeFlag flags, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies);
};
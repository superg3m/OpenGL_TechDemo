#pragma once

#include <shader.hpp>
#include <geometry.hpp>
#include <material.hpp>

// TODO(Jovanni): Consider removing transform from Mesh, 
// as entities typically have transforms, not meshes themselves.
struct Mesh {
    Geometry geometry;
    Material material;
    GLenum draw_type = GL_TRIANGLES; // GL_TRIANGLES, GL_LINES, ...

    Mesh() = default;
    Mesh(Material material, Geometry geometry, GLenum draw_type = GL_TRIANGLES);

    void draw(GM_Matrix4 transform);
    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
};
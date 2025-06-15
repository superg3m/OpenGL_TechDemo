#pragma once

#include <shader.hpp>
#include <geometry.hpp>
#include <material.hpp>

// TODO(Jovanni): Consider removing transform from Mesh, 
// as entities typically have transforms, not meshes themselves.
struct Mesh {
    Geometry geometry;
    Material material;

    Mesh() = default;
    Mesh(Material material, Geometry geometry);

    void draw(GM_Matrix4 transform);
    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
};
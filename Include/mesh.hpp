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
    Mesh(Material material, Geometry geometry, GM_Matrix4 transform = gm_mat4_identity(), GLenum draw_type = GL_TRIANGLES);

    void draw();
    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);

    void setScale(float s);
    void setScale(float x, float y, float z, float);
    void setScale(GM_Vec3 s);

    void setRotaion(float theta, float x, float y, float z);
    void setRotaion(float theta, GM_Vec3 r);

    void setPosition(GM_Vec3 p);
    void setPosition(float x, float y, float z);
private:
    GM_Matrix4 transform;
};
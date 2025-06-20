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

    void draw();
};
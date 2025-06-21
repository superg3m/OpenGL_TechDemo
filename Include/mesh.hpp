#pragma once

#include <Shader.hpp>
#include <Geometry.hpp>
#include <Material.hpp>

// TODO(Jovanni): Consider removing transform from Mesh, 
// as entities typically have transforms, not meshes themselves.

struct Mesh {
    Geometry geometry;
    Material material;

    Mesh() = default;
    Mesh(Geometry geometry, Material material = Material());

    void draw();
};
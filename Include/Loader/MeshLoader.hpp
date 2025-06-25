#pragma once

#include <map>
#include <string>
#include <Mesh.hpp>

struct MeshLoader {
    static std::map<std::string, Mesh> meshes;

    static void registerMesh(std::string key, Mesh model);
    static Mesh getMesh(std::string key);
};
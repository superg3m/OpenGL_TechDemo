#pragma once

#include <map>
#include <string>
#include <Mesh.hpp>

struct MeshLoader {
    static std::vector<std::string> mesh_keys;
    static std::map<std::string, Mesh> meshes;

    static void registerMesh(std::string key, Mesh mesh);
    static Mesh getMesh(std::string key);
};
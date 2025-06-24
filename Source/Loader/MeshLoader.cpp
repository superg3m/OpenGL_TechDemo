#include <ckg.h>
#include <MeshLoader.hpp>

std::vector<std::string> MeshLoader::mesh_keys;
std::map<std::string, Mesh> MeshLoader::meshes;

void MeshLoader::registerMesh(std::string key, Mesh mesh) {
    if (MeshLoader::meshes.count(key)) {
        CKG_LOG_WARN("MeshLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    MeshLoader::mesh_keys.push_back(key);
    MeshLoader::meshes[key] = mesh;
}

Mesh MeshLoader::getMesh(std::string key) {
    if (!MeshLoader::meshes.count(key)) {
        CKG_LOG_ERROR("MeshLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
    }

    return MeshLoader::meshes[key];
}
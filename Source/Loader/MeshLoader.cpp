#include <ckg.h>
#include <MeshLoader.hpp>

std::map<std::string, Mesh> MeshLoader::meshes;

void MeshLoader::registerMesh(std::string key, Mesh mesh) {
    if (MeshLoader::meshes.count(key)) {
        CKG_LOG_WARN("ModelLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    MeshLoader::meshes[key] = mesh;
}

Mesh MeshLoader::getMesh(std::string key) {
    if (!MeshLoader::meshes.count(key)) {
        CKG_LOG_ERROR("ModelLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
    }

    return MeshLoader::meshes[key];
}
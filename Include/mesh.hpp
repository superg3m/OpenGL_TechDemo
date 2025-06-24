#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Shader.hpp>
#include <Geometry.hpp>
#include <Material.hpp>

// TODO(Jovanni): Consider removing transform from Mesh, 
// as entities typically have transforms, not meshes themselves.

enum BufferType {
    INDEX_BUFFER = 0,
    VERTEX_BUFFER = 1,
    MVP_MAT_BUFFER = 2,
    WORLD_MAT_BUFFER = 3,
    BUFFER_COUNT = 4
};

#define INVALID_MATERIAL 0xFFFFFFFF

struct MeshEntry {
    GM_Vec3 position;
    GM_Quaternion orientation;
    GM_Vec3 scale;

    unsigned int vertex_count;
    unsigned int index_count;

    unsigned int material_index = INVALID_MATERIAL;
};

struct Mesh {
    unsigned int VAO;

    GM_Vec3 position;
    GM_Quaternion orientation;
    GM_Vec3 scale;
    Geometry geometry;

    std::vector<MeshEntry> meshes;
    std::vector<Material> materials;
    unsigned int SSBOs[BUFFER_COUNT];

    Mesh();
    Mesh(Geometry geometry, VertexAttributeFlag flags = VertexAttributeFlag::PNTBundle, Material material = Material());
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, VertexAttributeFlag flags = VertexAttributeFlag::PNTBundle, Material material = Material());
    Mesh(const std::string &path, unsigned int assimp_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    virtual void draw();
    virtual void draw(unsigned int instance_count);

    void setPosition(GM_Vec3 position);
    void setPosition(float x, float y, float z);

    void setOrientation(GM_Quaternion orientation);
    void setEulerAngles(GM_Vec3 euler);
    void setEulerAngles(float theta_x, float theta_y, float theta_z);

    void setScale(float scale);
    void setScale(GM_Vec3 scale);
    void setScale(float scale_x, float scale_y, float scale_z);

    GM_Matrix4 getTransform();
    // GM_AABB getAABB();
private:
    GM_AABB base_aabb;
    void loadMeshFromScene(const std::string &path, const aiScene* scene);
    void loadMeshFromData(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, VertexAttributeFlag flags);

    void setup(VertexAttributeFlag flags, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};
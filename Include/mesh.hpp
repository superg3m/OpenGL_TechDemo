#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Vertex.hpp>
#include <Geometry.hpp>
#include <ShaderBase.hpp>
#include <TextureLoader.hpp>

enum BufferType {
    INDEX_BUFFER = 0,
    VERTEX_BUFFER = 1,
    MVP_MAT_BUFFER = 2,
    WORLD_MAT_BUFFER = 3,
    BUFFER_COUNT = 4
};

#define INVALID_MATERIAL 0xFFFFFFFF

struct MeshEntry {
    unsigned int vertex_count = 0;
    unsigned int index_count  = 0;
    unsigned int base_vertex  = 0; // offset to next vertex in the vertex buffer
    unsigned int base_index   = 0; // offset to next index in the index buffer
    unsigned int material_index = INVALID_MATERIAL;
    GM_Matrix4 absolute_transform;
};

struct Mesh {
    unsigned int VAO;

    GM_Vec3 position;
    GM_Quaternion orientation;
    GM_Vec3 scale;
    unsigned int texture_flags = TEXTURE_DEFAULT;

    std::vector<MeshEntry> meshes;
    std::vector<Material> materials;
    unsigned int buffers[BUFFER_COUNT];
    bool should_render_aabb = false;
    GLenum draw_type = GL_TRIANGLES; // GL_TRIANGLES, GL_LINES

    Mesh();
    Mesh(Geometry geometry);
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, VertexAttributeFlag flags = VertexAttributeFlag::PNTBundle);
    Mesh(const std::string &path, unsigned int texture_flags = TEXTURE_DEFAULT, unsigned int assimp_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    void draw(ShaderBase &shader, bool useMaterial = false);

    void setPosition(GM_Vec3 p);
    void setPosition(float x, float y, float z);

    void setOrientation(GM_Quaternion orient);
    void setEulerAngles(GM_Vec3 euler);
    void setEulerAngles(float theta_x, float theta_y, float theta_z);

    void setScale(float scalar);
    void setScale(GM_Vec3 s);
    void setScale(float scale_x, float scale_y, float scale_z);

    GM_Matrix4 getTransform();
    GM_Matrix4 getAABBTransform();
    GM_AABB getAABB();

    static Mesh* Quad();
    static Mesh* AABB();
    static Mesh* Cube();
    static Mesh* Sphere(int segments);
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    GM_AABB base_aabb;
    void loadMeshFromScene(const std::string &path, const aiScene* scene);
    void loadMeshFromData(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, VertexAttributeFlag flags);

    MeshEntry processMesh(aiMesh *mesh, const aiScene *scene, GM_Matrix4 absolute_transform);
    void processNode(aiNode* node, const aiScene* scene, GM_Matrix4 parent_transform);

    void setup(VertexAttributeFlag flags);
};
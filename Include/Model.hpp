#pragma once

#include <glad/glad.h> 
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Mesh.hpp>
#include <Shader.hpp>

struct Model : public Mesh {
    std::vector<Mesh> meshes;

    Model(std::string const &path);
    void draw(Shader &shader) override;
private:
    std::string directory;
    std::map<std::string, GLTextureID> loaded_textures;

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<GLTextureID> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

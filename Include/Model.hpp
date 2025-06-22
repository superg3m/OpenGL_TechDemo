#pragma once

#include <glad/glad.h> 
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Mesh.hpp>
#include <Shader.hpp>
#include <TextureLoader.hpp>

struct Model : public Mesh {
    std::vector<Mesh> meshes;

    Model(std::string const &path, int texture_flags = TEXTURE_DEFAULT);
    void draw(Shader &shader, bool should_draw_textures = true) override;
private:
    std::string directory;
    std::map<std::string, GLTextureID> loaded_textures;
    int texture_flags;

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<GLTextureID> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

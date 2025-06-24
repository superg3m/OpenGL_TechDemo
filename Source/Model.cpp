#include <ckg.h>
#include <Model.hpp>
#include <ModelLoader.hpp>

Model::Model() {
    this->directory = "";
    this->geometry = Geometry();
    this->loaded_textures = {};
    this->texture_flags = TEXTURE_DEFAULT;
    this->loaded_textures = {};
    this->meshes = {};
}

Model::Model(std::string const &path, int texture_flags) {
    this->texture_flags = texture_flags;

    if (ModelLoader::models.count(path) != 0) {
        *this = ModelLoader::getModel(path);
        return;
    }

    this->loadModel(path);
    ModelLoader::registerModel(path, *this);
}

void Model::draw(Shader &shader, bool should_draw_textures) {
    for(unsigned int i = 0; i < this->meshes.size(); i++) {
        this->meshes[i].draw(shader, should_draw_textures);
    }
}

void Model::loadModel(std::string const &path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        CKG_LOG_ERROR("ASSIMP ERROR: %s\n", importer.GetErrorString());
        return;
    }

    this->directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        GM_Vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.aPosition = vector;

        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.aNormal = vector;
        }

        if(mesh->mTextureCoords[0]) {
            GM_Vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.aTexCoord = vec;

            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.aTangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.aBitangent = vector;
        } else {
            vertex.aTexCoord = GM_Vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);     
        }
    }

    aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];

    Mesh ret = Mesh();
    std::vector<GLTextureID> diffuseMaps = loadMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
    std::vector<GLTextureID> specularMaps = loadMaterialTextures(aiMaterial, aiTextureType_SPECULAR, "texture_specular");
    // std::vector<GLTextureID> normalMaps = loadMaterialTextures(aiMaterial, aiTextureType_HEIGHT, "texture_normal");
    // std::vector<GLTextureID> heightMaps = loadMaterialTextures(aiMaterial, aiTextureType_AMBIENT, "texture_height");

    Geometry geometry = Geometry(VertexAttributeFlag::PNTBundle, vertices, indices);
    Material material = Material();
    float opacity = 1.0f;
    if (aiMaterial->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
        material.opacity = opacity;
    }

    material.textures["uMaterial.diffuse"] = diffuseMaps[0];
    material.textures["uMaterial.specular"] = specularMaps[0];
    // material.textures["uMaterial.normal"] = normalMaps[0];
    // material.textures["uMaterial.height"] = heightMaps[0];
    
    return Mesh(geometry, material);
}

std::vector<GLTextureID> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string key) {
    std::vector<GLTextureID> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        if(loaded_textures.count(key) != 0) {
            textures.push_back(this->loaded_textures.at(key));
            skip = true;
        }

        if(!skip) {
            std::string filename = this->directory + '/' + std::string(str.C_Str());
            GLTextureID texture = TextureLoader::loadTexture(filename.c_str(), this->texture_flags);
            textures.push_back(texture);
            this->loaded_textures[key] = texture;
        }
    }

    if (textures.size() == 0) {
        textures.push_back(0);
    }

    return textures;
}

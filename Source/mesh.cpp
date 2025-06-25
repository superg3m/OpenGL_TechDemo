#include <TextureLoader.hpp>
#include <Mesh.hpp>

static std::map<TextureUnitType, aiTextureType> textureTypeToAssimpType = {
    { TEXTURE_UNIT_DIFFUSE, aiTextureType_DIFFUSE },
    { TEXTURE_UNIT_SPECULAR, aiTextureType_SPECULAR },
    { TEXTURE_UNIT_NORMAL, aiTextureType_NORMALS }, // Standard normal maps
    { TEXTURE_UNIT_METALNESS, aiTextureType_METALNESS },
    { TEXTURE_UNIT_EMISSIVE, aiTextureType_EMISSIVE },
    { TEXTURE_UNIT_NORMAL_CAMERA, aiTextureType_NORMAL_CAMERA }, // Direct match in newer Assimp
    { TEXTURE_UNIT_EMISSION_COLOR, aiTextureType_EMISSION_COLOR }, // Direct match in newer Assimp
    { TEXTURE_UNIT_ROUGHNESS, aiTextureType_DIFFUSE_ROUGHNESS }, // Common for PBR roughness
    { TEXTURE_UNIT_AMBIENT_OCCLUSION, aiTextureType_AMBIENT_OCCLUSION }
    // TEXTURE_COUNT does not map to an Assimp texture type
};

Mesh::Mesh() {
    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::identity();
    this->scale = GM_Vec3(1, 1, 1);
    this->base_aabb = GM_AABB(GM_Vec3(0, 0, 0), GM_Vec3(0, 0, 0));
    this->materials.reserve(1);
}

Mesh::Mesh(Geometry geometry) {
    this->VAO = geometry.VAO;
    this->SSBOs[VERTEX_BUFFER] = geometry.VBO;
    this->SSBOs[INDEX_BUFFER] = geometry.EBO;
    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::identity();
    this->scale = GM_Vec3(1, 1, 1);
    this->materials.reserve(1);

    // this->base_aabb = this->base_aabb_from_vertices(vertices);
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, VertexAttributeFlag flags) {
    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::identity();
    this->scale = GM_Vec3(1, 1, 1);
    this->materials.reserve(1);
    // this->base_aabb = this->base_aabb_from_vertices(vertices);

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glGenBuffers(ArrayCount(this->SSBOs), this->SSBOs);

    this->loadMeshFromData(vertices, indices, flags);
}

Mesh::Mesh(const std::string &path, unsigned int assimp_flags) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, assimp_flags);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        CKG_LOG_ERROR("ASSIMP ERROR: %s | File: %s\n", importer.GetErrorString(), path);
        return;
    }

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glGenBuffers(ArrayCount(this->SSBOs), this->SSBOs);

    this->loadMeshFromScene(path, scene);
}

void Mesh::loadMeshFromScene(const std::string &path, const aiScene* scene) {
    this->meshes.reserve(scene->mNumMeshes);

    std::vector<Material> materials;
    materials.reserve(scene->mNumMaterials);

    unsigned int total_vertex_count = 0;
    unsigned int total_index_count = 0;
    for (unsigned int i = 0 ; i < this->meshes.size() ; i++) {
        this->meshes[i].material_index = scene->mMeshes[i]->mMaterialIndex;
        this->meshes[i].index_count = scene->mMeshes[i]->mNumFaces * 3;
        this->meshes[i].base_vertex = total_vertex_count;
        this->meshes[i].base_index = total_index_count;

        total_vertex_count += scene->mMeshes[i]->mNumVertices;
        total_index_count  += this->meshes[i].index_count;
    }

    std::vector<Vertex> vertices;
    vertices.reserve(total_vertex_count);

    std::vector<unsigned int> indices;
    indices.reserve(total_index_count);

    for (unsigned int i = 0 ; i < this->meshes.size() ; i++) {
        const aiMesh* paiMesh = scene->mMeshes[i];
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

        for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
            Vertex v = Vertex();
            const aiVector3D& position = paiMesh->mVertices[i];

            v.aPosition = GM_Vec3(position.x, position.y, position.z);

            if (paiMesh->mNormals) {
                const aiVector3D& pNormal = paiMesh->mNormals[i];
                v.aNormal = GM_Vec3(pNormal.x, pNormal.y, pNormal.z);
            } else {
                aiVector3D Normal(0.0f, 1.0f, 0.0f);
                v.aNormal = GM_Vec3(Normal.x, Normal.y, Normal.z);
            }

            const aiVector3D& uv = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;
            v.aTexCoord = GM_Vec2(uv.x, uv.y);

            vertices.push_back(v);
        }

        for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
            const aiFace& Face = paiMesh->mFaces[i];
            indices.push_back(Face.mIndices[0]);
            indices.push_back(Face.mIndices[1]);
            indices.push_back(Face.mIndices[2]);
        }
    }

    std::string directory = path.substr(0, path.find_last_of('/'));

    for (unsigned int i = 0 ; i < scene->mNumMaterials ; i++) {
        const aiMaterial* ai_material = scene->mMaterials[i];

        float opacity = 1.0f;
        if (ai_material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
            this->materials[i].opacity = opacity;
        } else {
            CKG_LOG_WARN("Mesh Failed opacity matkey?\n");
        }

        for (int type_int = 0; type_int < TEXTURE_COUNT; ++type_int) {
            TextureUnitType type = static_cast<TextureUnitType>(type_int);

            if (textureTypeToAssimpType.count(type) == 0) {
                CKG_LOG_WARN("Skipping TextureUnitType: %s | Reason: has no Assimp mapping.\n", texture_to_string[type]);
                continue;
            }
    
            aiTextureType ai_type = textureTypeToAssimpType.at(type);
            if (ai_material->GetTextureCount(ai_type) <= 0) {
                CKG_LOG_WARN("Material: %d | has no texture of type: %s\n", i, texture_to_string[type]);
                continue;
            }

            aiString str;
            if (ai_material->GetTexture(ai_type, 0, &str, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string filename = directory + '/' + std::string(str.C_Str());

                if (TextureLoader::textures.count(filename) == 0) {
                    TextureLoader::registerTexture(filename, filename.c_str());
                }

                this->materials[i].textures[type] = TextureLoader::textures.at(filename);
            } else {
                CKG_LOG_ERROR("Failed to get texture path for material: %d | type: %s\n", i, texture_to_string[type]);
            }
        }
    }

    setup(VertexAttributeFlag::PNTBundle, vertices, indices);
}

void Mesh::loadMeshFromData(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, VertexAttributeFlag flags) {
    
    MeshEntry entry;
    entry.vertex_count = vertices.size();
    entry.index_count = indices.size();

    this->meshes.reserve(1);

    setup(VertexAttributeFlag::PNTBundle, vertices, indices);
}

/*
GM_AABB Mesh::getAABB() {
    // find_min from verticies
    return GM_AABB::fromCenterExtents(this->position, this->scale);
}
*/

void Mesh::setPosition(GM_Vec3 position) {
    this->position = position;
}

void Mesh::setPosition(float x, float y, float z) {
    this->position = GM_Vec3(x, y, z);
}

void Mesh::setOrientation(GM_Quaternion orientation) {
    this->orientation = orientation;
}

void Mesh::setEulerAngles(GM_Vec3 euler) {
    this->orientation = GM_Quaternion::fromEuler(euler);
}

void Mesh::setEulerAngles(float theta_x, float theta_y, float theta_z) {
    this->orientation = GM_Quaternion::fromEuler(GM_Vec3(theta_x, theta_y, theta_z));
}

void Mesh::setScale(float scale) {
    this->scale = GM_Vec3(scale, scale, scale);
}

void Mesh::setScale(GM_Vec3 scale) {
    this->scale = scale;
}

void Mesh::setScale(float scale_x, float scale_y, float scale_z) {
    this->scale = GM_Vec3(scale_x, scale_y, scale_z);
}

GM_Matrix4 Mesh::getTransform() {
    GM_Matrix4 transform = GM_Matrix4::identity();
    transform = GM_Matrix4::scale(transform, this->scale);
    transform = GM_Matrix4::rotate(transform, this->orientation);
    transform = GM_Matrix4::translate(transform, this->position);

    return transform;
}

void Mesh::setup(VertexAttributeFlag flags, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    glBindBuffer(GL_ARRAY_BUFFER, this->SSBOs[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->SSBOs[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    bool hasPosition   = hasVertexAttributeFlag(flags, VertexAttributeFlag::aPosition);
    bool hasNormal     = hasVertexAttributeFlag(flags, VertexAttributeFlag::aNormal);
    bool hasTexCoord   = hasVertexAttributeFlag(flags, VertexAttributeFlag::aTexCoord);
    bool hasTanget     = hasVertexAttributeFlag(flags, VertexAttributeFlag::aTangent);
    bool hasBitanget   = hasVertexAttributeFlag(flags, VertexAttributeFlag::aBitangent);
    bool hasColor      = hasVertexAttributeFlag(flags, VertexAttributeFlag::aColor);
    bool hasBoneID     = hasVertexAttributeFlag(flags, VertexAttributeFlag::aBoneIDs);
    bool hasBoneWeight = hasVertexAttributeFlag(flags, VertexAttributeFlag::aBoneWeights);
    size_t offset = 0;

    for (const auto& desc : ALL_ATTRIBUTE_DESCRIPTORS) {
        if (hasVertexAttributeFlag(flags, desc.flag)) {
            glEnableVertexAttribArray(desc.location);
            if (desc.isInteger) {
                glVertexAttribIPointer(desc.location, desc.componentCount, desc.glType, sizeof(Vertex), (void*)offset);
            } else {
                glVertexAttribPointer(desc.location, desc.componentCount, desc.glType, desc.normalized, sizeof(Vertex), (void*)offset);
            }

            offset += desc.byteSize;
        } else {
            glDisableVertexAttribArray(desc.location);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/*
GM_Matrix4 Mesh::getAABBTransform() {
    GM_AABB aabb = this->getAABB();
    GM_Matrix4 transform = GM_Matrix4::identity();
    transform = GM_Matrix4::scale(transform, aabb.getExtents().scale(1.005f)); // scale to stop z-fighting
    transform = GM_Matrix4::rotate(transform, this->orientation);
    transform = GM_Matrix4::translate(transform, aabb.getCenter());

    return transform;
}
*/

void Mesh::draw() {
    glBindVertexArray(this->VAO);

    for (unsigned int mesh_index = 0 ; mesh_index < this->meshes.size() ; mesh_index++) {
        unsigned int material_index = this->meshes[mesh_index].material_index;

        glDrawElementsBaseVertex(
            GL_TRIANGLES, this->meshes[mesh_index].index_count, 
            GL_UNSIGNED_INT, 
            (void*)(sizeof(unsigned int) * this->meshes[mesh_index].base_index), 
            this->meshes[mesh_index].base_index
        );
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}
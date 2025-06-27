#include <TextureLoader.hpp>
#include <MeshLoader.hpp>
#include <Mesh.hpp>

static std::map<TextureType, aiTextureType> textureTypeToAssimpType = {
    { TEXTURE_TYPE_DIFFUSE, aiTextureType_DIFFUSE },
    { TEXTURE_TYPE_SPECULAR, aiTextureType_SPECULAR },
    { TEXTURE_TYPE_NORMAL, aiTextureType_NORMALS },
    { TEXTURE_TYPE_METALNESS, aiTextureType_METALNESS },
    { TEXTURE_TYPE_EMISSIVE, aiTextureType_EMISSIVE },
    { TEXTURE_TYPE_NORMAL_CAMERA, aiTextureType_NORMAL_CAMERA },
    { TEXTURE_TYPE_EMISSION_COLOR, aiTextureType_EMISSION_COLOR },
    { TEXTURE_TYPE_ROUGHNESS, aiTextureType_DIFFUSE_ROUGHNESS },
    { TEXTURE_TYPE_AMBIENT_OCCLUSION, aiTextureType_AMBIENT_OCCLUSION }
};

Mesh::Mesh() {
    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::identity();
    this->scale = GM_Vec3(1, 1, 1);
    this->base_aabb = GM_AABB(GM_Vec3(0, 0, 0), GM_Vec3(0, 0, 0));
    this->materials.resize(1);
}

Mesh::Mesh(Geometry geometry) {
    this->VAO = geometry.VAO;
    this->buffers[VERTEX_BUFFER] = geometry.VBO;
    this->buffers[INDEX_BUFFER] = geometry.EBO;

    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::identity();
    this->scale = GM_Vec3(1, 1, 1);
    this->should_render_aabb = false;

    this->materials.resize(1);
    this->meshes.resize(1);

    this->draw_type = geometry.draw_type;
    this->meshes[0].vertex_count = geometry.vertex_count;
    this->meshes[0].index_count  = geometry.index_count;
    this->meshes[0].base_vertex  = 0;
    this->meshes[0].base_index   = 0;
    this->meshes[0].material_index = 0;
    // this->base_aabb = this->base_aabb_from_vertices(vertices);
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, VertexAttributeFlag flags) {
    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::identity();
    this->scale = GM_Vec3(1, 1, 1);
    // this->base_aabb = this->base_aabb_from_vertices(vertices);

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glGenBuffers(ArrayCount(this->buffers), this->buffers);

    this->loadMeshFromData(vertices, indices, flags);
}

Mesh::Mesh(const std::string &path, unsigned int texture_flags, unsigned int assimp_flags) {
    if (MeshLoader::meshes.count(path)) {
        *this = MeshLoader::getMesh(path);
        return;
    }

    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::identity();
    this->scale = GM_Vec3(1, 1, 1);
    this->texture_flags = texture_flags;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, assimp_flags);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        CKG_LOG_ERROR("ASSIMP ERROR: %s\n", importer.GetErrorString());
        return;
    }

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glGenBuffers(ArrayCount(this->buffers), this->buffers);

    CKG_LOG_DEBUG("Loading: %s\n", path.c_str());
    this->loadMeshFromScene(path, scene);
    MeshLoader::registerMesh(path, *this);
}

void Mesh::loadMeshFromScene(const std::string &path, const aiScene* scene) {
    { // geometry
        this->meshes.resize(scene->mNumMeshes);
        this->materials.resize(scene->mNumMaterials);

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
            const aiMesh* ai_mesh = scene->mMeshes[i];
            const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

            Vertex v = Vertex();
            for (unsigned int j = 0; j < ai_mesh->mNumVertices; j++) {
                const aiVector3D& ai_position = ai_mesh->mVertices[j];

                v.aPosition = GM_Vec3(ai_position.x, ai_position.y, ai_position.z);

                if (ai_mesh->mNormals) {
                    const aiVector3D& pNormal = ai_mesh->mNormals[j];
                    v.aNormal = GM_Vec3(pNormal.x, pNormal.y, pNormal.z);
                } else {
                    aiVector3D Normal(0.0f, 1.0f, 0.0f);
                    v.aNormal = GM_Vec3(Normal.x, Normal.y, Normal.z);
                }

                const aiVector3D& uv = ai_mesh->HasTextureCoords(0) ? ai_mesh->mTextureCoords[0][j] : Zero3D;
                v.aTexCoord = GM_Vec2(uv.x, uv.y);

                vertices.push_back(v);
            }

            for (unsigned int j = 0; j < ai_mesh->mNumFaces; j++) {
                const aiFace& Face = ai_mesh->mFaces[j];
                if (Face.mNumIndices == 3) {
                    indices.push_back(Face.mIndices[0]);
                    indices.push_back(Face.mIndices[1]);
                    indices.push_back(Face.mIndices[2]);
                } else {
                    CKG_LOG_ERROR("Mesh '%s' has non-triangular face with %d indices. Skipping.\n", ai_mesh->mName.C_Str(), Face.mNumIndices);
                }
            }
        }    
        setup(VertexAttributeFlag::PNTBundle, vertices, indices);
    } // geometry end

    { // materials start
        std::string directory = path.substr(0, path.find_last_of('/'));
        for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
            const aiMaterial* ai_material = scene->mMaterials[i];

            aiColor4D ambient_color(0.0f, 0.0f, 0.0f, 0.0f);
            GM_Vec3 white = GM_Vec3(1.0f);

            if (ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color) == AI_SUCCESS) {
                this->materials[i].ambient_color.r = ambient_color.r;
                this->materials[i].ambient_color.g = ambient_color.g;
                this->materials[i].ambient_color.b = ambient_color.b;
            } else {
                this->materials[i].ambient_color = white;
            }

            aiColor3D diffuse_color(0.0f, 0.0f, 0.0f);

            if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color) == AI_SUCCESS) {
                this->materials[i].diffuse_color.r = diffuse_color.r;
                this->materials[i].diffuse_color.g = diffuse_color.g;
                this->materials[i].diffuse_color.b = diffuse_color.b;
            }

            aiColor3D specular_color(0.0f, 0.0f, 0.0f);

            if (ai_material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color) == AI_SUCCESS) {
                this->materials[i].specular_color.r = specular_color.r;
                this->materials[i].specular_color.g = specular_color.g;
                this->materials[i].specular_color.b = specular_color.b;
            }

            float opacity = 1.0f;
            if (ai_material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
                this->materials[i].opacity = opacity;
            } else {
                CKG_LOG_WARN("Mesh Failed opacity matkey?\n");
            }

            for (int type_int = 0; type_int < TEXTURE_COUNT; type_int++) {
                TextureType type = static_cast<TextureType>(type_int);

                if (textureTypeToAssimpType.count(type) == 0) {
                    continue;
                }

                aiTextureType ai_type = textureTypeToAssimpType.at(type);
                if (ai_material->GetTextureCount(ai_type) <= 0) {
                    continue;
                }

                aiString str;
                if (ai_material->GetTexture(ai_type, 0, &str, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                    std::string texture_id_or_path = std::string(str.C_Str());
                    const aiTexture* ai_texture = scene->GetEmbeddedTexture(str.C_Str());

                    if (ai_texture) {
                        // int width, height, nrChannel = 0;
                        // u8* image_data = stbi_load_from_memory((u8*)ai_texture->pcData, ai_texture->mWidth, &width, &height, &nrChannel, 0);
                        // GLTextureID id = TextureLoader::loadTextureFromMemory(image_data, width, height, nrChannel);
                        // TextureLoader::registerTexture(texture_id_or_path, id);
                        CKG_LOG_DEBUG("Loaded embedded Texture\n");
                    } else {
                        std::string filename = directory + '/' + texture_id_or_path;
                        CKG_LOG_DEBUG("Material: %d | has external texture of type: %s\n", i, texture_to_string[type]);
                        if (TextureLoader::textures.count(filename) == 0) {
                            TextureLoader::registerTexture(filename, filename.c_str(), this->texture_flags);
                        }
                        this->materials[i].textures[type] = TextureLoader::textures.at(filename);
                    }
                } else {
                    CKG_LOG_ERROR("Failed to get texture path for material: %d | type: %s\n", i, texture_to_string[type]);
                }
            }
        }
    } // materials end
}

void Mesh::loadMeshFromData(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, VertexAttributeFlag flags) {
    this->materials.resize(1);
    this->meshes.resize(1);
    this->meshes[0].vertex_count = (unsigned int)vertices.size();
    this->meshes[0].index_count  = (unsigned int)indices.size();
    this->meshes[0].base_vertex  = 0;
    this->meshes[0].base_index   = 0;
    this->meshes[0].material_index = 0;

    setup(flags, vertices, indices);
}

void Mesh::setPosition(GM_Vec3 p) {
    this->position = p;
}

void Mesh::setPosition(float x, float y, float z) {
    this->position = GM_Vec3(x, y, z);
}

void Mesh::setOrientation(GM_Quaternion orient) {
    this->orientation = orient;
}

void Mesh::setEulerAngles(GM_Vec3 euler) {
    this->orientation = GM_Quaternion::fromEuler(euler);
}

void Mesh::setEulerAngles(float theta_x, float theta_y, float theta_z) {
    this->orientation = GM_Quaternion::fromEuler(GM_Vec3(theta_x, theta_y, theta_z));
}

void Mesh::setScale(float scalar) {
    this->scale = GM_Vec3(scalar, scalar, scalar);
}

void Mesh::setScale(GM_Vec3 s) {
    this->scale = s;
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



GM_Matrix4 Mesh::getAABBTransform() {
    GM_AABB aabb = this->getAABB();
    GM_Matrix4 transform = GM_Matrix4::identity();
    transform = GM_Matrix4::scale(transform, aabb.getExtents().scale(1.005f)); // scale to stop z-fighting
    transform = GM_Matrix4::rotate(transform, this->orientation);
    transform = GM_Matrix4::translate(transform, aabb.getCenter());

    return transform;
}

GM_AABB Mesh::getAABB() {
    // find_min from verticies
    return GM_AABB::fromCenterExtents(this->position, this->scale);
}

void Mesh::setup(VertexAttributeFlag flags, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // bool hasPosition   = hasVertexAttributeFlag(flags, VertexAttributeFlag::aPosition);
    // bool hasNormal     = hasVertexAttributeFlag(flags, VertexAttributeFlag::aNormal);
    // bool hasTexCoord   = hasVertexAttributeFlag(flags, VertexAttributeFlag::aTexCoord);
    // bool hasTanget     = hasVertexAttributeFlag(flags, VertexAttributeFlag::aTangent);
    // bool hasBitanget   = hasVertexAttributeFlag(flags, VertexAttributeFlag::aBitangent);
    // bool hasColor      = hasVertexAttributeFlag(flags, VertexAttributeFlag::aColor);
    // bool hasBoneID     = hasVertexAttributeFlag(flags, VertexAttributeFlag::aBoneIDs);
    // bool hasBoneWeight = hasVertexAttributeFlag(flags, VertexAttributeFlag::aBoneWeights);
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

void Mesh::draw(ShaderBase &shader, bool useMaterial) {
    glBindVertexArray(this->VAO);

    for (unsigned int mesh_index = 0 ; mesh_index < this->meshes.size() ; mesh_index++) {
        if (useMaterial) {
            unsigned int material_index = this->meshes[mesh_index].material_index;
            shader.setMaterial(this->materials[material_index]);
        }

        if (this->meshes[mesh_index].index_count > 0) {
            glDrawElementsBaseVertex(
                draw_type, this->meshes[mesh_index].index_count, 
                GL_UNSIGNED_INT, 
                (void*)(sizeof(unsigned int) * this->meshes[mesh_index].base_index), 
                this->meshes[mesh_index].base_vertex
            );
        } else {
            glDrawArrays(
                draw_type,
                this->meshes[mesh_index].base_vertex,
                this->meshes[mesh_index].vertex_count
            );
        }

        shader.unbindTextureUnits();
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}
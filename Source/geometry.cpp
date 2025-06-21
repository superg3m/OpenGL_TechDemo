#include <geometry.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GeometryLoader.hpp>

#define RESERVED_QUAD   "ReservedQuad"
#define RESERVED_CUBE   "ReservedCube"
#define RESERVED_AABB   "ReservedAABB"
#define RESERVED_SPHERE "ReservedSphere"

Geometry Geometry::Quad() {
    if (GeometryLoader::geometry.count(RESERVED_QUAD) != 0) {
        return GeometryLoader::getGeometry(RESERVED_QUAD);
    }

    std::vector<Vertex> quad_vertices = {
        //         Posrtion             Normal          UV
        Vertex{GM_Vec3( 1.0f,  1.0f, 0.0f),  GM_Vec3(1.0f, 0.0f, 0.0f),  GM_Vec2(1, 1)}, // top right
        Vertex{GM_Vec3( 1.0f, -1.0f, 0.0f),  GM_Vec3(0.0f, 1.0f, 0.0f),  GM_Vec2(1, 0)}, // bottom right
        Vertex{GM_Vec3(-1.0f, -1.0f, 0.0f),  GM_Vec3(0.0f, 0.0f, 1.0f),  GM_Vec2(0, 0)}, // bottom left
        Vertex{GM_Vec3(-1.0f,  1.0f, 0.0f),  GM_Vec3(1.0f, 1.0f, 0.0f),  GM_Vec2(0, 1)}  // top left 
    }; 

    std::vector<unsigned int> quad_indices = {
        0, 1, 3,
        1, 2, 3
    };

    Geometry ret;
    ret.draw_type = GL_TRIANGLES;
    ret.setup(VertexAttributeFlag::PNTBundle, quad_vertices, quad_indices);

    GeometryLoader::registerGeometry(RESERVED_QUAD, ret);

    return ret;
}

Geometry Geometry::AABB() {
    if (GeometryLoader::geometry.count(RESERVED_AABB) != 0) {
        return GeometryLoader::getGeometry(RESERVED_AABB);
    }

    std::vector<Vertex> aabb_vertices = {
        // Bottom face
        Vertex{GM_Vec3(-1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3( 1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3( 1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3( 1.0f, -1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3( 1.0f, -1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3(-1.0f, -1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3(-1.0f, -1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3(-1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},

        // Top GM_Vec3(face
        Vertex{GM_Vec3(-1.0f, 1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3( 1.0f, 1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3( 1.0f, 1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3( 1.0f, 1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3( 1.0f, 1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3(-1.0f, 1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3(-1.0f, 1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3(-1.0f, 1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},

        // Vertical edges
        Vertex{GM_Vec3(-1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3(-1.0f, 1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3( 1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3( 1.0f, 1.0f, -1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3( 1.0f, -1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3( 1.0f, 1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3(-1.0f, -1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)}, Vertex{GM_Vec3(-1.0f, 1.0f,  1.0f), GM_Vec3(0, 0, 0), GM_Vec2(0, 0)},
    };

    Geometry ret;
    ret.draw_type = GL_LINES;
    ret.setup(VertexAttributeFlag::PNTBundle, aabb_vertices, {});

    GeometryLoader::registerGeometry(RESERVED_AABB, ret);

    return ret;
}


Geometry Geometry::Cube() {
    if (GeometryLoader::geometry.count(RESERVED_CUBE) != 0) {
        return GeometryLoader::getGeometry(RESERVED_CUBE);
    }

    std::vector<Vertex> cube_vertices = {
        // Front face
        Vertex{GM_Vec3(-1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, -1), GM_Vec2(0, 0)}, // 0
        Vertex{GM_Vec3( 1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, -1), GM_Vec2(1, 0)}, // 1
        Vertex{GM_Vec3( 1.0f,  1.0f, -1.0f), GM_Vec3(0, 0, -1), GM_Vec2(1, 1)}, // 2
        Vertex{GM_Vec3(-1.0f,  1.0f, -1.0f), GM_Vec3(0, 0, -1), GM_Vec2(0, 1)}, // 3

        // Back face
        Vertex{GM_Vec3(-1.0f, -1.0f, 1.0f), GM_Vec3(0, 0, 1), GM_Vec2(0, 0)}, // 4
        Vertex{GM_Vec3( 1.0f, -1.0f, 1.0f), GM_Vec3(0, 0, 1), GM_Vec2(1, 0)}, // 5
        Vertex{GM_Vec3( 1.0f,  1.0f, 1.0f), GM_Vec3(0, 0, 1), GM_Vec2(1, 1)}, // 6
        Vertex{GM_Vec3(-1.0f,  1.0f, 1.0f), GM_Vec3(0, 0, 1), GM_Vec2(0, 1)}, // 7

        // Left face
        Vertex{GM_Vec3(-1.0f, -1.0f,  1.0f), GM_Vec3(-1, 0, 0), GM_Vec2(0, 0)}, // 8
        Vertex{GM_Vec3(-1.0f, -1.0f, -1.0f), GM_Vec3(-1, 0, 0), GM_Vec2(1, 0)}, // 9
        Vertex{GM_Vec3(-1.0f,  1.0f, -1.0f), GM_Vec3(-1, 0, 0), GM_Vec2(1, 1)}, // 10
        Vertex{GM_Vec3(-1.0f,  1.0f,  1.0f), GM_Vec3(-1, 0, 0), GM_Vec2(0, 1)}, // 11

        // Right face
        Vertex{GM_Vec3( 1.0f, -1.0f, -1.0f), GM_Vec3(1, 0, 0), GM_Vec2(0, 0)}, // 12
        Vertex{GM_Vec3( 1.0f, -1.0f,  1.0f), GM_Vec3(1, 0, 0), GM_Vec2(1, 0)}, // 13
        Vertex{GM_Vec3( 1.0f,  1.0f,  1.0f), GM_Vec3(1, 0, 0), GM_Vec2(1, 1)}, // 14
        Vertex{GM_Vec3( 1.0f,  1.0f, -1.0f), GM_Vec3(1, 0, 0), GM_Vec2(0, 1)}, // 15

        // Bottom face
        Vertex{GM_Vec3(-1.0f, -1.0f, -1.0f), GM_Vec3(0, -1, 0), GM_Vec2(0, 1)}, // 16
        Vertex{GM_Vec3( 1.0f, -1.0f, -1.0f), GM_Vec3(0, -1, 0), GM_Vec2(1, 1)}, // 17
        Vertex{GM_Vec3( 1.0f, -1.0f,  1.0f), GM_Vec3(0, -1, 0), GM_Vec2(1, 0)}, // 18
        Vertex{GM_Vec3(-1.0f, -1.0f,  1.0f), GM_Vec3(0, -1, 0), GM_Vec2(0, 0)}, // 19

        // Top face
        Vertex{GM_Vec3(-1.0f,  1.0f, -1.0f), GM_Vec3(0, 1, 0), GM_Vec2(0, 1)}, // 20
        Vertex{GM_Vec3( 1.0f,  1.0f, -1.0f), GM_Vec3(0, 1, 0), GM_Vec2(1, 1)}, // 21
        Vertex{GM_Vec3( 1.0f,  1.0f,  1.0f), GM_Vec3(0, 1, 0), GM_Vec2(1, 0)}, // 22
        Vertex{GM_Vec3(-1.0f,  1.0f,  1.0f), GM_Vec3(0, 1, 0), GM_Vec2(0, 0)}, // 23
    };

    std::vector<unsigned int> cube_indices = {
        0,  1,  2,  2,  3,  0,       // Front
        4,  5,  6,  6,  7,  4,       // Back
        8,  9,  10, 10, 11, 8,       // Left
        12, 13, 14, 14, 15, 12,      // Right
        16, 17, 18, 18, 19, 16,      // Bottom
        20, 21, 22, 22, 23, 20       // Top
    };

    Geometry ret;
    ret.draw_type = GL_TRIANGLES;
    ret.setup(VertexAttributeFlag::PNTBundle, cube_vertices, cube_indices);

    GeometryLoader::registerGeometry(RESERVED_CUBE, ret);

    return ret;
}

Geometry Geometry::Sphere(int segments) {
    if (GeometryLoader::geometry.count(RESERVED_SPHERE) != 0) {
        return GeometryLoader::getGeometry(RESERVED_SPHERE);
    }

    std::vector<Vertex> sphere_vertices;
    std::vector<unsigned int> sphere_indices;

    int rings = segments;
    int sectors = segments;
    float TWO_PI = 2.0f * PI;

    for (int r = 0; r <= rings; ++r) {
        float phi = PI * r / rings;
        float y = cosf(phi);
        float sin_phi = sinf(phi);

        for (int s = 0; s <= sectors; ++s) {
            float theta = TWO_PI * s / sectors;
            float x = sin_phi * cosf(theta);
            float z = sin_phi * sinf(theta);

            float u = static_cast<float>(s) / sectors;
            float v = static_cast<float>(r) / rings;

            sphere_vertices.push_back(Vertex{
                GM_Vec3(x, y, z),
                GM_Vec3(x, y, z),                      // Normal (unit direction)
                GM_Vec2(u, v)                          // UV
            });
        }
    }

    for (int r = 0; r < rings; ++r) {
        for (int s = 0; s < sectors; ++s) {
            int i0 = r * (sectors + 1) + s;
            int i1 = i0 + 1;
            int i2 = i0 + (sectors + 1);
            int i3 = i2 + 1;

            // Triangle 1
            sphere_indices.push_back(i0);
            sphere_indices.push_back(i2);
            sphere_indices.push_back(i1);

            // Triangle 2
            sphere_indices.push_back(i1);
            sphere_indices.push_back(i2);
            sphere_indices.push_back(i3);
        }
    }

    Geometry ret;
    ret.draw_type = GL_TRIANGLES;
    ret.setup(VertexAttributeFlag::PNTBundle, sphere_vertices, sphere_indices);

    GeometryLoader::registerGeometry(RESERVED_SPHERE, ret);

    return ret;
}

/*
Geometry Geometry::Model(const char* path) {
    if (GeometryLoader::geometry.count(RESERVED_SPHERE) != 0) {
        return GeometryLoader::getGeometry(RESERVED_SPHERE);
    }

    auto processMesh = [](aiMesh *mesh, const aiScene *scene) -> Mesh {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        // std::vector<Texture> textures;

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            GM_Vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
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
            // texture coordinates
            if(mesh->mTextureCoords[0]) {
                GM_Vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.aTexCoord = vec;

                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.aTangent = vector;

                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.aBitangent = vector;
            }
            else {
                vertex.aTexCoord = GM_Vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);   
            }
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    };

    auto processNode = []() -> void {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++) {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path) {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    Geometry ret;
    ret.vertices = sphere_vertices;
    ret.indices = sphere_indices;
    ret.draw_type = GL_TRIANGLES;
    ret.setup(VertexAttributeFlag::PNTBundle);

    return ret;
}
*/

void Geometry::setup(VertexAttributeFlag flags, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    this->vertex_count = vertices.size();
    this->index_count = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertex_count * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    if (!indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index_count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

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

Geometry::Geometry(VertexAttributeFlag flags, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, GLenum draw_type) {
    this->draw_type = draw_type;
    this->setup(flags, vertices, indices);
}
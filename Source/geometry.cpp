#include <geometry.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GeometryLoader.hpp>

#define RESERVED_QUAD   "ReservedQuad"
#define RESERVED_CUBE   "ReservedCube"
#define RESERVED_AABB   "ReservedAABB"
#define RESERVED_SPHERE "ReservedSphere"

Geometry::Geometry() {
    this->VAO = 0;
    this->VBO = 0;
    this->EBO = 0;

    this->draw_type = GL_TRIANGLES;
    this->vertex_count = 0;
    this->index_count = 0;
}

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
        Vertex{GM_Vec3(-1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, -1), GM_Vec2(0, 0)},
        Vertex{GM_Vec3( 1.0f, -1.0f, -1.0f), GM_Vec3(0, 0, -1), GM_Vec2(1, 0)},
        Vertex{GM_Vec3( 1.0f,  1.0f, -1.0f), GM_Vec3(0, 0, -1), GM_Vec2(1, 1)},
        Vertex{GM_Vec3(-1.0f,  1.0f, -1.0f), GM_Vec3(0, 0, -1), GM_Vec2(0, 1)},

        // Back face
        Vertex{GM_Vec3(-1.0f, -1.0f, 1.0f), GM_Vec3(0, 0, 1), GM_Vec2(0, 0)},
        Vertex{GM_Vec3( 1.0f, -1.0f, 1.0f), GM_Vec3(0, 0, 1), GM_Vec2(1, 0)},
        Vertex{GM_Vec3( 1.0f,  1.0f, 1.0f), GM_Vec3(0, 0, 1), GM_Vec2(1, 1)},
        Vertex{GM_Vec3(-1.0f,  1.0f, 1.0f), GM_Vec3(0, 0, 1), GM_Vec2(0, 1)},

        // Left face
        Vertex{GM_Vec3(-1.0f, -1.0f,  1.0f), GM_Vec3(-1, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3(-1.0f, -1.0f, -1.0f), GM_Vec3(-1, 0, 0), GM_Vec2(1, 0)},
        Vertex{GM_Vec3(-1.0f,  1.0f, -1.0f), GM_Vec3(-1, 0, 0), GM_Vec2(1, 1)},
        Vertex{GM_Vec3(-1.0f,  1.0f,  1.0f), GM_Vec3(-1, 0, 0), GM_Vec2(0, 1)},

        // Right face
        Vertex{GM_Vec3( 1.0f, -1.0f, -1.0f), GM_Vec3(1, 0, 0), GM_Vec2(0, 0)},
        Vertex{GM_Vec3( 1.0f, -1.0f,  1.0f), GM_Vec3(1, 0, 0), GM_Vec2(1, 0)},
        Vertex{GM_Vec3( 1.0f,  1.0f,  1.0f), GM_Vec3(1, 0, 0), GM_Vec2(1, 1)},
        Vertex{GM_Vec3( 1.0f,  1.0f, -1.0f), GM_Vec3(1, 0, 0), GM_Vec2(0, 1)},

        // Bottom face
        Vertex{GM_Vec3(-1.0f, -1.0f, -1.0f), GM_Vec3(0, -1, 0), GM_Vec2(0, 1)},
        Vertex{GM_Vec3( 1.0f, -1.0f, -1.0f), GM_Vec3(0, -1, 0), GM_Vec2(1, 1)},
        Vertex{GM_Vec3( 1.0f, -1.0f,  1.0f), GM_Vec3(0, -1, 0), GM_Vec2(1, 0)},
        Vertex{GM_Vec3(-1.0f, -1.0f,  1.0f), GM_Vec3(0, -1, 0), GM_Vec2(0, 0)},

        // Top face
        Vertex{GM_Vec3(-1.0f,  1.0f, -1.0f), GM_Vec3(0, 1, 0), GM_Vec2(0, 1)},
        Vertex{GM_Vec3( 1.0f,  1.0f, -1.0f), GM_Vec3(0, 1, 0), GM_Vec2(1, 1)},
        Vertex{GM_Vec3( 1.0f,  1.0f,  1.0f), GM_Vec3(0, 1, 0), GM_Vec2(1, 0)},
        Vertex{GM_Vec3(-1.0f,  1.0f,  1.0f), GM_Vec3(0, 1, 0), GM_Vec2(0, 0)},
    };

    std::vector<unsigned int> cube_indices = {
        0,  1,  2,  2,  3,  0,  // Front
        4,  5,  6,  6,  7,  4,  // Back
        8,  9,  10, 10, 11, 8,  // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Bottom
        20, 21, 22, 22, 23, 20  // Top
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

void Geometry::setup(VertexAttributeFlag flags, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    this->vertex_count = (unsigned int)vertices.size();
    this->index_count = (unsigned int)indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertex_count * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index_count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

Geometry::Geometry(VertexAttributeFlag flags, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, GLenum draw_type) {
    this->draw_type = draw_type;
    this->setup(flags, vertices, indices);
}
#pragma once

#include <glad/glad.h>
#include <gm.hpp>

struct Vertex {
    GM_Vec3 aPosition;    // location 0
    GM_Vec3 aNormal;      // location 1
    GM_Vec2 aTexCoord;    // location 2
    GM_Vec3 aTangent;     // location 3
    GM_Vec3 aBitangent;   // location 4
    GM_Vec4 aColor;       // location 5
    GM_Vec4 aBoneIDs;     // location 6 (use GM_IVec4 if you have it for integer IDs)
    GM_Vec4 aBoneWeights; // location 7 (using vec4 for weights for simplicity)

    // Default constructor to initialize, or use default member initializers if C++11+
    Vertex() {
        this->aPosition    = GM_Vec3(0, 0, 0);
        this->aNormal      = GM_Vec3(0, 1, 0);
        this->aTexCoord    = GM_Vec2(0, 0);
        this->aTangent     = GM_Vec3(1, 0, 0);
        this->aBitangent   = GM_Vec3(0, 0, 1);
        this->aColor       = GM_Vec4(1, 1, 1, 1);
        this->aBoneIDs     = GM_Vec4(0, 0, 0, 0);
        this->aBoneWeights = GM_Vec4(0, 0, 0, 0);
    }

    Vertex(GM_Vec3 pos) : Vertex() { 
        this->aPosition = pos;
    }

    Vertex(GM_Vec3 pos, GM_Vec3 norm, GM_Vec2 uv) : Vertex(pos) {
        this->aNormal = norm; 
        this->aTexCoord = uv;
    }

    Vertex(GM_Vec3 pos, GM_Vec3 norm, GM_Vec2 uv, GM_Vec3 tangent, GM_Vec3 bitangent) : Vertex(pos, norm, uv) {
        this->aTangent = tangent;
        this->aBitangent = bitangent;
    }

    Vertex(GM_Vec3 pos, GM_Vec3 norm, GM_Vec2 uv, GM_Vec3 tangent, GM_Vec3 bitangent, GM_Vec4 color) : Vertex(pos, norm, uv, tangent, bitangent) {
        this->aColor = color;
    }

    Vertex(GM_Vec3 pos, GM_Vec3 norm, GM_Vec2 uv, GM_Vec3 tangent, GM_Vec3 bitangent, GM_Vec4 color, GM_Vec4 boneIDs, GM_Vec4 bone_weights) : Vertex(pos, norm, uv, tangent, bitangent, color) {
        this->aBoneIDs = boneIDs;
        this->aBoneWeights = bone_weights;
    }
};

enum class VertexAttributeFlag : uint32_t {
    aPosition     = 0x1,
    aNormal       = 0x2,
    aTexCoord     = 0x4,
    aTangent      = 0x8,
    aBitangent    = 0x10,
    aColor        = 0x20,
    aBoneIDs      = 0x40,
    aBoneWeights  = 0x80,

    PNTBundle = aPosition | aNormal | aTexCoord,
    TangentBundle = PNTBundle | aTangent | aBitangent,
    ColorBundle = TangentBundle | aColor,
    All = ColorBundle | aBoneIDs | aBoneWeights,
};

inline VertexAttributeFlag operator|(VertexAttributeFlag a, VertexAttributeFlag b) {
    return static_cast<VertexAttributeFlag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
inline bool hasVertexAttributeFlag(VertexAttributeFlag value, VertexAttributeFlag flag) {
    return (static_cast<uint32_t>(value) & static_cast<uint32_t>(flag)) == static_cast<uint32_t>(flag);
}

struct AttributeDescriptor {
    VertexAttributeFlag flag;
    int location;
    int componentCount;       // e.g., 3 for vec3, 4 for vec4
    GLenum glType;            // e.g., GL_FLOAT, GL_INT
    bool normalized;
    bool isInteger;           // True if using glVertexAttribIPointer
    size_t byteSize;
};

static const std::vector<AttributeDescriptor> ALL_ATTRIBUTE_DESCRIPTORS = {
    {VertexAttributeFlag::aPosition,    0, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3},
    {VertexAttributeFlag::aNormal,      1, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3},
    {VertexAttributeFlag::aTexCoord,    2, 2, GL_FLOAT, GL_FALSE, false, sizeof(float) * 2},
    {VertexAttributeFlag::aTangent,     3, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3},
    {VertexAttributeFlag::aBitangent,   4, 3, GL_FLOAT, GL_FALSE, false, sizeof(float) * 3},
    {VertexAttributeFlag::aColor,       5, 4, GL_FLOAT, GL_FALSE, false, sizeof(float) * 4}, // Or GL_UNSIGNED_BYTE, true, true
    {VertexAttributeFlag::aBoneIDs,     6, 4, GL_INT,   GL_FALSE, true,  sizeof(int)   * 4}, // Use GL_INT and isInteger=true
    {VertexAttributeFlag::aBoneWeights, 7, 4, GL_FLOAT, GL_FALSE, false, sizeof(float) * 4}
};
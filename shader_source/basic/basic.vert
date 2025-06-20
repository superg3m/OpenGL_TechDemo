#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in vec4 aColor;
layout(location = 6) in ivec4 aBoneIDs;
layout(location = 7) in vec4 aBoneWeights;

uniform int attribute_flags;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 boneTransforms[MAX_BONES];

#define HAS_POSITION_FLAG   (1 << 0)
#define HAS_NORMAL_FLAG     (1 << 1)
#define HAS_TEXCOORD_FLAG   (1 << 2)
#define HAS_TANGENT_FLAG    (1 << 3)
#define HAS_BITANGENT_FLAG  (1 << 4)
#define HAS_COLOR_FLAG      (1 << 5)
#define HAS_BONES_FLAG      ((1 << 7) | (1 << 6))

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Tangent;
out vec3 Bitangent;
out vec4 Color;

void main() {
    vec3 finalPos = aPos;
    vec3 finalNormal = aNormal;
    vec3 finalTangent = aTangent;
    vec3 finalBitangent = aBitangent;

    if ((attribute_flags & HAS_BONES_FLAG) != 0) {
        mat4 boneTransform = boneTransforms[aBoneIDs.x] * aBoneWeights.x;
        boneTransform += boneTransforms[aBoneIDs.y] * aBoneWeights.y;
        boneTransform += boneTransforms[aBoneIDs.z] * aBoneWeights.z;
        boneTransform += boneTransforms[aBoneIDs.w] * aBoneWeights.w;

        finalPos = vec3(boneTransform * vec4(aPos, 1.0));
        finalNormal = normalize(mat3(boneTransform) * aNormal);
        finalTangent = normalize(mat3(boneTransform) * aTangent);
        finalBitangent = normalize(mat3(boneTransform) * aBitangent);
    }

    FragPos = vec3(model * vec4(finalPos, 1.0));
    if ((attribute_flags & HAS_NORMAL_FLAG) != 0) {
        Normal = normalize(mat3(model) * finalNormal);
    } else {
        Normal = vec3(0.0, 1.0, 0.0);
    }

    if ((attribute_flags & HAS_TANGENT_FLAG) != 0) {
        Tangent = normalize(mat3(model) * finalTangent);
        if ((attribute_flags & HAS_BITANGENT_FLAG) != 0) {
            Bitangent = normalize(mat3(model) * finalBitangent);
        } else {
            Bitangent = normalize(cross(Normal, Tangent));
        }
    } else {
        Tangent = vec3(1.0, 0.0, 0.0);
        Bitangent = vec3(0.0, 0.0, 1.0);
    }

    if ((attribute_flags & HAS_TEXCOORD_FLAG) != 0U) {
        TexCoords = aTexCoords;
    } else {
        TexCoords = vec2(0.0, 0.0);
    }

    if ((attribute_flags & HAS_COLOR_FLAG) != 0U) {
        Color = aColor;
    } else {
        Color = vec4(1.0);
    }

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
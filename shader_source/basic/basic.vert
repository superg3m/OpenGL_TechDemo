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
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

#define HAS_POSITION_BIT   (1 << 0)
#define HAS_NORMAL_BIT     (1 << 1) 
#define HAS_TEXCOORD_BIT   (1 << 2)
#define HAS_TANGENT_BIT    (1 << 3)
#define HAS_BITANGENT_BIT  (1 << 4)
#define HAS_COLOR_BIT      (1 << 5)
#define HAS_BONES_BIT      ((1 << 7) | (1 << 6))

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

out VS_OUT {
    vec2 texCoords;
} vs_out;


void main() {
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(uModel))) * aNormal;
    TexCoords = aTexCoords;
    vs_out.texCoords = aTexCoords;
    
    gl_Position = uProjection * uView * vec4(FragPos, 1.0);
}
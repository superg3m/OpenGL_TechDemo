#pragma once

#include <string>
#include <gm.hpp>

using GLTextureID = int;
struct PBR_Material {
    float roughness = 0.0f;
    bool isMetal = false;
    GM_Vec4 color = GM_Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    GLTextureID albedo_map = 0;
    GLTextureID roughness_map = 0;
    GLTextureID metallic_map = 0;
    GLTextureID normal_map = 0;
    GLTextureID AO_map = 0;
    GLTextureID emissive_map = 0;
};

enum TextureType {
    TEXTURE_TYPE_DIFFUSE = 0,
    TEXTURE_TYPE_SPECULAR = 1,
    TEXTURE_TYPE_NORMAL = 2,
    TEXTURE_TYPE_METALNESS = 3,
    TEXTURE_TYPE_EMISSIVE = 4,
    TEXTURE_TYPE_NORMAL_CAMERA = 5,
    TEXTURE_TYPE_EMISSION_COLOR = 6,
    TEXTURE_TYPE_ROUGHNESS = 7,
    TEXTURE_TYPE_AMBIENT_OCCLUSION = 8,
    TEXTURE_COUNT = 9
};

static const char* texture_to_string[TEXTURE_COUNT] = {
    stringify(TEXTURE_TYPE_DIFFUSE),
    stringify(TEXTURE_TYPE_SPECULAR),
    stringify(TEXTURE_TYPE_NORMAL),
    stringify(TEXTURE_TYPE_METALNESS),
    stringify(TEXTURE_TYPE_EMISSIVE),
    stringify(TEXTURE_TYPE_NORMAL_CAMERA),
    stringify(TEXTURE_TYPE_EMISSION_COLOR),
    stringify(TEXTURE_TYPE_ROUGHNESS),
    stringify(TEXTURE_TYPE_AMBIENT_OCCLUSION),
};

struct Material {
    std::string name;
    GLTextureID textures[TEXTURE_COUNT];
    GM_Vec3 color = GM_Vec3(1);
    float shininess = 32.0f;
    float opacity = 1.0f;

    GM_Vec3 ambient_color  = GM_Vec3(0);
    GM_Vec3 diffuse_color  = GM_Vec3(0);
    GM_Vec3 specular_color = GM_Vec3(0);

    Material();

    void assignTexture(TextureType type, GLTextureID id);
};
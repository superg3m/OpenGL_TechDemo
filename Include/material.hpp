#pragma once

#include <vector>

#include <Shader.hpp>
#include <gm.hpp>

struct PBR_Material {
    float roughness = 0.0f;
    bool isMetal = false;
    GM_Vec3 color = GM_Vec3(0.0f, 0.0f, 0.0f);
    GLTextureID albedo_map = 0;
    GLTextureID roughness_map = 0;
    GLTextureID metallic_map = 0;
    GLTextureID normal_map = 0;
    GLTextureID AO_map = 0;
    GLTextureID emissive_map = 0;
};

enum TextureUnitType {
    TEXTURE_UNIT_DIFFUSE = 0,
    TEXTURE_UNIT_SPECULAR = 1,
    TEXTURE_UNIT_NORMAL = 2,
    TEXTURE_UNIT_METALNESS = 3,
    TEXTURE_UNIT_EMISSIVE = 4,
    TEXTURE_UNIT_NORMAL_CAMERA = 5,
    TEXTURE_UNIT_EMISSION_COLOR = 6,
    TEXTURE_UNIT_ROUGHNESS = 7,
    TEXTURE_UNIT_AMBIENT_OCCLUSION = 8,
    TEXTURE_COUNT = 9
};

static const char* texture_to_string[TEXTURE_COUNT] = {
    stringify(TEXTURE_UNIT_DIFFUSE),
    stringify(TEXTURE_UNIT_SPECULAR),
    stringify(TEXTURE_UNIT_NORMAL),
    stringify(TEXTURE_UNIT_METALNESS),
    stringify(TEXTURE_UNIT_EMISSIVE),
    stringify(TEXTURE_UNIT_NORMAL_CAMERA),
    stringify(TEXTURE_UNIT_EMISSION_COLOR),
    stringify(TEXTURE_UNIT_ROUGHNESS),
    stringify(TEXTURE_UNIT_AMBIENT_OCCLUSION),
};

struct Material {
    std::string name;

    GM_Vec4 color;
    float opacity = 1.0f;
    float alpha_test = 0.0f;

    GLTextureID textures[TEXTURE_COUNT];

    Material();
    void bindTexture(TextureUnitType type, GLTextureID id);
};
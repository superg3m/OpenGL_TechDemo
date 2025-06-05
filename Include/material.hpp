#pragma once

#include <gm.h>
#include <glad/glad.h>
#include <shader.hpp>
#include <vector>

enum TextureType {
    TEXTURE_COLOR,
    TEXTURE_DECAL,
    TEXTURE_SPECULAR,
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT,
    TEXTURE_ALPHA,
    TEXTURE_MAX
};

enum TextureFlag {
    TEXTURE_DEFAULT = 0x0,
    TEXTURE_PIXEL_PERFECT = 0x1,
    TEXTURE_VERTICAL_FLIP = 0x2
};

using GLTextureID = int;
struct Material {
    Shader* shader;
    GM_RGBA color;
    GLTextureID textures[TEXTURE_MAX] = {0};

    Material() = default;
    Material(Shader* shader);

    void bindTextures();
    void unbindTextures();

    static Material BasicMaterial();
    static Material StandardMaterial();
};
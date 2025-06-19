#pragma once

#include <gm.hpp>
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
    TEXTURE_CUBEMAP,
    TEXTURE_MAX,

    TEXTURE_INVALID = -1,
};

using GLTextureID = int;
struct Material {
    Shader shader;
    GLTextureID textures[TEXTURE_MAX];
    GM_Vec4 color;

    Material() = default;
    Material(Shader shader);

    void bindTextures();
    void unbindTextures();

    static Material BasicMaterial();
    static Material StandardMaterial();
};
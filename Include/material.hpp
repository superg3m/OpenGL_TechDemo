#pragma once

#include <vector>

#include <glad/glad.h>
#include <Shader.hpp>

#include <gm.hpp>

using GLTextureID = int;
struct Material {
    GM_Vec4 color;
    std::map<std::string, GLTextureID> textures;

    Material();

    void bindTextures();
    void unbindTextures();
};
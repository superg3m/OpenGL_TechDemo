#pragma once

#include <gm.hpp>
#include <glad/glad.h>
#include <shader.hpp>
#include <vector>

using GLTextureID = int;
struct Material {
    GM_Vec4 color;
    std::map<std::string, GLTextureID> textures;

    Material();

    void bindTextures();
    void unbindTextures();
};
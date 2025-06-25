#include <ckg.h>
#include <material.hpp>

Material::Material() {
    this->opacity = 1.0f;
    this->color = GM_Vec4(1, 1, 1, 1);
    ckg_memory_fill(this->textures, ArrayCount(this->textures), 0);
}

void Material::bindTexture(TextureType type, GLTextureID id) {
    this->textures[type] = id;
}
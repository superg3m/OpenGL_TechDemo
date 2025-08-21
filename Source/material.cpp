#include <ckg.h>
#include <Material.hpp>

Material::Material() {
    this->opacity = 1.0f;
    this->color = GM_Vec3(1, 1, 1);
    ckg_memory_fill(this->textures, ArrayCount(this->textures), 0);
}

void Material::assignTexture(TextureType type, GLTextureID id) {
    this->textures[type] = id;
}
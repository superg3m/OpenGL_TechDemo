#include <material.hpp>

Material::Material(Shader shader) {
    for (int i = 0; i < TEXTURE_MAX; i++) {
        this->textures[i] = TEXTURE_INVALID;
    }
    
    this->shader = shader;
}

void Material::bindTextures() {
    this->shader.use();
    for (int i = 0; i < TEXTURE_MAX; i++) {
        if (i == TEXTURE_COLOR) {
            this->shader.setInt("material.mainTexture", 0);
        } else if (i == TEXTURE_DECAL && (this->textures[i] != TEXTURE_INVALID)) {
            this->shader.setInt("material.decalTexture", 1);
            this->shader.setBool("material.hasDecal", true);
        }

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i]);
    }
}

void Material::unbindTextures() {
    this->shader.use();
    for (int i = 0; i < TEXTURE_MAX; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

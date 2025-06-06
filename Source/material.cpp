#include <material.hpp>

Material::Material(Shader* shader) {
    this->shader = shader;
}

void Material::bindTextures() {
    this->shader->use();
    for (int i = 0; i < TEXTURE_MAX; i++) {
        if (i == TEXTURE_COLOR) {
            this->shader->setInt("texture1", 0);
        } else if (i == TEXTURE_DECAL) {
            this->shader->setInt("texture2", 1);
        }

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i]);
    }
}

void Material::unbindTextures() {
    this->shader->use();
    for (int i = 0; i < TEXTURE_MAX; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

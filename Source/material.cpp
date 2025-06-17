#include <material.hpp>
#include <string>

Material::Material(Shader shader) {
    for (int i = 0; i < TEXTURE_MAX; i++) {
        this->textures[i] = TEXTURE_INVALID;
    }
    
    this->shader = shader;
}

void Material::bindTextures() {
    this->shader.use();
    for (int TEXTURE_TYPE = 0; TEXTURE_TYPE < TEXTURE_MAX; TEXTURE_TYPE++) {
        bool hasValidTexture = this->textures[TEXTURE_TYPE] != TEXTURE_INVALID;
        std::string textureBuilder = std::string("texture") + std::to_string(TEXTURE_TYPE);
        std::string boolBuilder    = std::string("hasTexture") + std::to_string(TEXTURE_TYPE);
        this->shader.setInt(textureBuilder.c_str(), TEXTURE_TYPE);
        this->shader.setBool(boolBuilder.c_str(), hasValidTexture);

        if (!hasValidTexture) {
            continue;
        }

        glActiveTexture(GL_TEXTURE0 + TEXTURE_TYPE);
        glBindTexture(GL_TEXTURE_2D, this->textures[TEXTURE_TYPE]);

        if (TEXTURE_TYPE == TEXTURE_CUBEMAP) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, this->textures[TEXTURE_TYPE]);
        }
    }
}

void Material::unbindTextures() {
    this->shader.use();
    for (int i = 0; i < TEXTURE_MAX; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

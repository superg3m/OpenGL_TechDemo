#include <ShaderSkybox.hpp>

ShaderSkybox::ShaderSkybox(std::vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderSkybox::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);
}

void ShaderSkybox::setSkyboxTexture(GLTextureID texture) const {
    this->use();

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glUniform1i(this->getUniformLocation("uSkyboxTexture"), 0); 
}
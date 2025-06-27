#include <ShaderSkybox.hpp>

void ShaderSkybox::init() {
    std::vector<const char*> shader_paths = {"../../shader_source/skybox/skybox.vert", "../../shader_source/skybox/skybox.frag"};
    this->program_id = this->createShaderProgram(shader_paths);
}

void ShaderSkybox::setSkyboxTexture(GLTextureID texture) const {
    this->use();

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glUniform1i(this->getUniformLocation("uSkyboxTexture"), 0); 
}
#include <ShaderTransparency.hpp>

void ShaderTransparency::init() {
    std::vector<const char*> shader_paths = {"../../shader_source/transparency/t.vert", "../../shader_source/transparency/t.frag"};
    this->program_id = this->createShaderProgram(shader_paths);
}

void ShaderTransparency::setTexture(GLTextureID texture) const {
    this->use();

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(this->getUniformLocation("uTexture"), 0); 
}

void ShaderTransparency::setOpacity(float opacity) const {
    this->use();

    glUniform1f(this->getUniformLocation("uOpacity"), opacity); 
}
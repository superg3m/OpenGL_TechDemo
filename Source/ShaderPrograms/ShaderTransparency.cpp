#include <ShaderTransparency.hpp>

ShaderTransparency::ShaderTransparency(std::vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderTransparency::compile() {
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
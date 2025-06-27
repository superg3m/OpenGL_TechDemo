#include <ShaderStencilOutline.hpp>

ShaderStencilOutline::ShaderStencilOutline(std::vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->program_id = this->createShaderProgram(shader_paths);
}

void ShaderStencilOutline::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);

    this->uOutlineScale_Location = this->getUniformLocation("uOutlineScale");
}

void ShaderStencilOutline::setOutlineScale(float s) const {
    this->use();

    glUniform1f(this->uOutlineScale_Location, s); 
}
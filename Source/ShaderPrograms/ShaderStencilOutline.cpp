#include <ShaderStencilOutline.hpp>

void ShaderStencilOutline::init() {
    std::vector<const char*> shader_paths = {"../../shader_source/outline/outline.vert", "../../shader_source/outline/outline.frag"};
    this->program_id = this->createShaderProgram(shader_paths);

    this->uOutlineScale_Location = this->getUniformLocation("uOutlineScale");
}

void ShaderStencilOutline::setOutlineScale(float s) const {
    this->use();

    glUniform1f(this->uOutlineScale_Location, s); 
}
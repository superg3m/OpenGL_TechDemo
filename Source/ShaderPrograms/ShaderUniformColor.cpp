#include <ShaderUniformColor.hpp>

ShaderUniformColor::ShaderUniformColor(std::vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderUniformColor::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);
    this->uColor_Location = getUniformLocation("uColor");
}

void ShaderUniformColor::setColor(GM_Vec3 color) {
    this->use();

    glUniform3fv(this->uColor_Location, 1, &color.r);
}
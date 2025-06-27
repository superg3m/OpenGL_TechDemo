#include <ShaderUniformColor.hpp>

ShaderUniformColor::ShaderUniformColor() {
    std::vector<const char*> shader_paths = {"../../shader_source/model/model.vert", "../../shader_source/model/model.frag"};
    this->program_id = this->createShaderProgram(shader_paths);
    this->uColor_Location = getUniformLocation("uColor");
}

void ShaderUniformColor::setColor(GM_Vec3 &color) {
    this->use();

    glUniform3fv(this->uColor_Location, 1, &color.r);
}
#include <ShaderUniformColor.hpp>

ShaderUniformColor::ShaderUniformColor() {
    std::vector<const char*> shader_paths = {"../../shader_source/model/model.vert", "../../shader_source/model/model.frag"};
    this->program_id = create_shader_program(shader_paths);

    this->uModel_Location = shader_get_uniform_location(this->program_id, "uModel", shader_paths[0]);
    this->uView_Location = shader_get_uniform_location(this->program_id, "uView", shader_paths[0]);
    this->uProjection_Location = shader_get_uniform_location(this->program_id, "uProjection", shader_paths[0]);

    this->uMaterial_Location = shader_get_uniform_location(this->program_id, "uMaterial", shader_paths[0]);
}

void ShaderUniformColor::use() {
    glUseProgram(this->program_id);
}

void ShaderUniformColor::setModel(GM_Matrix4 &model) {
    this->use();
    glUniformMatrix4fv(this->uModel_Location, 1, GL_TRUE, &model.v[0].x);
}

void ShaderUniformColor::setView(GM_Matrix4 &view) {
    this->use();
    glUniformMatrix4fv(this->uView_Location, 1, GL_TRUE, &view.v[0].x);
}

void ShaderUniformColor::setProjection(GM_Matrix4 &projection) {
    this->use();

    glUniformMatrix4fv(this->uProjection_Location, 1, GL_TRUE, &projection.v[0].x);
}

void ShaderUniformColor::setColor(GM_Vec3 &color) {
    this->use();

    glUniform3fv(this->uColor_Location, 1, &color.r);
}
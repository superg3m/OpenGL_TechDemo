#include <ShaderUniformColor.hpp>

int ShaderUniformColor::getUniformLocation(const char* name, const char* path) const {
    GLint location = glGetUniformLocation(this->program_id, name);
    if (location == -1) {
        CKG_LOG_ERROR("Shader {%s} Uniform: '%s' does not exists\n", path, name);
    }

    return location;
}

ShaderUniformColor::ShaderUniformColor() {
    std::vector<const char*> shader_paths = {"../../shader_source/model/model.vert", "../../shader_source/model/model.frag"};
    this->program_id = create_shader_program(shader_paths);

    this->uModel_Location = this->getUniformLocation("uModel", shader_paths[0]);
    this->uView_Location = this->getUniformLocation("uView", shader_paths[0]);
    this->uProjection_Location = this->getUniformLocation("uProjection", shader_paths[0]);

    this->uMaterial_Location = this->getUniformLocation("uMaterial", shader_paths[0]);
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

void ShaderUniformColor::setMaterial(Material &material) {
    this->use();

    glUniform4fv(this->uMaterial_Location, 1, &material.color.x);
}
#include <ShaderSkybox.hpp>

int ShaderSkybox::getUniformLocation(const char* name, const char* path) const {
    GLint location = glGetUniformLocation(this->program_id, name);
    if (location == -1) {
        CKG_LOG_ERROR("Shader {%s} Uniform: '%s' does not exists\n", path, name);
    }

    return location;
}

ShaderSkybox::ShaderSkybox() {
    std::vector<const char*> shader_paths = {"../../shader_source/model/model.vert", "../../shader_source/model/model.frag"};
    this->program_id = create_shader_program(shader_paths);

    this->uView_Location = this->getUniformLocation("uView", shader_paths[0]);
    this->uProjection_Location = this->getUniformLocation("uProjection", shader_paths[0]);

    this->uSkyboxTexture_Location = this->getUniformLocation("uSkyboxTexture", shader_paths[0]);
}

void ShaderSkybox::use() {
    glUseProgram(this->program_id);
}

void ShaderSkybox::setView(GM_Matrix4 &view) {
    this->use();
    glUniformMatrix4fv(this->uView_Location, 1, GL_TRUE, &view.v[0].x);
}

void ShaderSkybox::setProjection(GM_Matrix4 &projection) {
    this->use();

    glUniformMatrix4fv(this->uProjection_Location, 1, GL_TRUE, &projection.v[0].x);
}

void ShaderSkybox::setSkyboxTexture(GLTextureID texture) {
    this->use();

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glUniform1i(this->uSkyboxTexture_Location, 0); 
}
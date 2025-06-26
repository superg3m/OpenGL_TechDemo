#include <vector>
#include <glad/glad.h>
#include <gm.hpp>
#include <material.hpp>
#include <ckg.h>
#include <shader.hpp>

#define LIGHT_COUNT 4

struct ShaderModel {
    ShaderModel();

    unsigned int program_id;

    void use() const;
    void setModel(GM_Matrix4 &model) const;
    void setView(GM_Matrix4 &view) const;
    void setProjection(GM_Matrix4 &projection) const;

    void setMaterial(Material &material) const;
    void setLightPosition(GM_Vec3 &position, int index) const;
    void setLightColor(GM_Vec3 &color, int index) const;
    void setViewPosition(GM_Vec3 &view_position) const;

    void setGamma(bool &gamma) const;
private:
    // Vertex Uniforms
    unsigned int uModel_Location;
    unsigned int uView_Location;
    unsigned int uProjection_Location;

    // Fragment Uniforms
    unsigned int uMaterial_Location;
    unsigned int uLightPosition_Locations[LIGHT_COUNT];
    unsigned int uLightColor_Locations[LIGHT_COUNT];
    unsigned int uViewPosition_Location;
    unsigned int uGamma_Location;

    int getUniformLocation(const char* name, const char* path) const;
};

ShaderModel::ShaderModel() {
    std::vector<const char*> shader_paths = {"../../shader_source/model/model.vert", "../../shader_source/model/model.frag"};
    this->program_id = create_shader_program(shader_paths);

    this->uModel_Location = this->getUniformLocation("uModel", shader_paths[0]);
    this->uView_Location = this->getUniformLocation("uView", shader_paths[0]);
    this->uProjection_Location = this->getUniformLocation("uProjection", shader_paths[0]);

    this->uMaterial_Location = this->getUniformLocation("uMaterial", shader_paths[0]);

    for (int i = 0; i < LIGHT_COUNT; i++) {
        this->uLightPosition_Locations[i] = this->getUniformLocation("lightPositions["+ std::to_string(i) + "]", shader_paths[0]);
        this->uLightColor_Locations[i] = this->getUniformLocation("lightColors["+ std::to_string(i) + "]", shader_paths[0]);
    }

    this->uViewPosition_Location = this->getUniformLocation("viewPosition", shader_paths[0]);
    this->uGamma_Location = this->getUniformLocation("gamma", shader_paths[0]);
}

void ShaderModel::use() const {
    glUseProgram(this->program_id);
}

void ShaderModel::setModel(GM_Matrix4 &model) const {
    this->use();
    glUniformMatrix4fv(this->uModel_Location, 1, GL_TRUE, &model.v[0].x);
}

void ShaderModel::setView(GM_Matrix4 &view) const {
    this->use();
    glUniformMatrix4fv(this->uView_Location, 1, GL_TRUE, &view.v[0].x);
}

void ShaderModel::setProjection(GM_Matrix4 &projection) const {
    this->use();

    glUniformMatrix4fv(this->uProjection_Location, 1, GL_TRUE, &projection.v[0].x);
}

void ShaderModel::setMaterial(Material &material) const {
    this->use();

    glUniform4fv(this->uMaterial_Location, 1, &material.color.x);
}

void ShaderModel::setLightPosition(GM_Vec3 &position, int index) const {
    this->use();

    glUniform3fv(this->uLightPosition_Locations[index], 1, &position.x);
}

void ShaderModel::setLightColor(GM_Vec3 &color, int index) const {
    this->use();

    glUniform3fv(this->uLightColor_Locations[index], 1, &color.x);
}

void ShaderModel::setViewPosition(GM_Vec3 &view_position) const {
    this->use();

    glUniform3fv(this->uViewPosition_Location, 1, &view_position.x);
}

void ShaderModel::setGamma(bool &gamma) const {
    this->use();

    glUniform1i(this->uGamma_Location, (int)gamma);
}
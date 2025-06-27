#include <ShaderModel.hpp>

ShaderModel::ShaderModel() {
    std::vector<const char*> shader_paths = {"../../shader_source/model/model.vert", "../../shader_source/model/model.frag"};
    this->program_id = this->createShaderProgram(shader_paths);

    for (int i = 0; i < LIGHT_COUNT; i++) {
        this->uLightPosition_Locations[i] = this->getUniformLocation("lightPositions["+ std::to_string(i) + "]");
        this->uLightColor_Locations[i] =this->getUniformLocation("lightColors["+ std::to_string(i) + "]");
    }

    this->uViewPosition_Location = this->getUniformLocation("viewPosition");
    this->uGamma_Location = this->getUniformLocation("gamma");
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
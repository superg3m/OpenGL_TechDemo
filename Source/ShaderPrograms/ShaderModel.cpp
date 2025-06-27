#include <ShaderModel.hpp>

void ShaderModel::init() {
    std::vector<const char*> shader_paths = {"../../shader_source/model/model.vert", "../../shader_source/model/model.frag"};
    this->program_id = this->createShaderProgram(shader_paths);

    for (int i = 0; i < LIGHT_COUNT; i++) {
        this->uPointLight_Locations[i].position = this->getUniformLocation("uPointLights["+ std::to_string(i) + "].position");

        this->uPointLight_Locations[i].constant = this->getUniformLocation("uPointLights["+ std::to_string(i) + "].constant");
        this->uPointLight_Locations[i].linear = this->getUniformLocation("uPointLights["+ std::to_string(i) + "].linear");
        this->uPointLight_Locations[i].quadratic = this->getUniformLocation("uPointLights["+ std::to_string(i) + "].quadratic");

        this->uPointLight_Locations[i].ambient = this->getUniformLocation("uPointLights["+ std::to_string(i) + "].ambient");
        this->uPointLight_Locations[i].diffuse = this->getUniformLocation("uPointLights["+ std::to_string(i) + "].diffuse");
        this->uPointLight_Locations[i].specular = this->getUniformLocation("uPointLights["+ std::to_string(i) + "].specular");
    }

    this->uViewPosition_Location = this->getUniformLocation("uViewPosition");
}

void ShaderModel::setPointLight(PointLight &point_light, int index) const {
    this->use();

    glUniform3fv(this->uPointLight_Locations[index].position, 1, &point_light.position.x);

    glUniform1f(this->uPointLight_Locations[index].constant, point_light.constant);
    glUniform1f(this->uPointLight_Locations[index].linear, point_light.linear);
    glUniform1f(this->uPointLight_Locations[index].quadratic, point_light.quadratic);

    glUniform3fv(this->uPointLight_Locations[index].ambient, 1, &point_light.ambient.x);
    glUniform3fv(this->uPointLight_Locations[index].diffuse, 1, &point_light.diffuse.x);
    glUniform3fv(this->uPointLight_Locations[index].specular, 1, &point_light.specular.x);
}

void ShaderModel::setViewPosition(GM_Vec3 &view_position) const {
    this->use();

    glUniform3fv(this->uViewPosition_Location, 1, &view_position.x);
}
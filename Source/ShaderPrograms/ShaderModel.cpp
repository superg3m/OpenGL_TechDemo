#include <ShaderModel.hpp>

ShaderModel::ShaderModel(std::vector<const char*> shader_paths) {
    this->shader_paths = shader_paths;
    this->compile();
}

void ShaderModel::compile() {
    this->program_id = this->createShaderProgram(this->shader_paths);

    this->uSpotLight_Location.position = this->getUniformLocation("uSpotLight.position");
    this->uSpotLight_Location.direction = this->getUniformLocation("uSpotLight.direction");
    this->uSpotLight_Location.cutOff = this->getUniformLocation("uSpotLight.cutOff");
    this->uSpotLight_Location.outerCutOff = this->getUniformLocation("uSpotLight.outerCutOff");
    this->uSpotLight_Location.constant = this->getUniformLocation("uSpotLight.constant");
    this->uSpotLight_Location.linear = this->getUniformLocation("uSpotLight.linear");
    this->uSpotLight_Location.diffuse = this->getUniformLocation("uSpotLight.quadratic");
    this->uSpotLight_Location.ambient = this->getUniformLocation("uSpotLight.ambient");
    this->uSpotLight_Location.diffuse = this->getUniformLocation("uSpotLight.diffuse");
    this->uSpotLight_Location.specular = this->getUniformLocation("uSpotLight.specular");

    this->uDirectionalLight_Location.direction = this->getUniformLocation("uDirLight.direction");
    this->uDirectionalLight_Location.ambient = this->getUniformLocation("uDirLight.ambient");
    this->uDirectionalLight_Location.diffuse = this->getUniformLocation("uDirLight.diffuse");
    this->uDirectionalLight_Location.specular = this->getUniformLocation("uDirLight.specular");

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
    this->uUseFlashlight_Location = this->getUniformLocation("uUseFlashlight");
}

void ShaderModel::setSpotLight(SpotLight &spot_light) const {
    this->use();

    glUniform3fv(this->uSpotLight_Location.position, 1, &spot_light.position.x);
    glUniform3fv(this->uSpotLight_Location.direction, 1, &spot_light.direction.x);
    glUniform1f(this->uSpotLight_Location.cutOff, spot_light.cutOff);
    glUniform1f(this->uSpotLight_Location.outerCutOff, spot_light.outerCutOff);

    glUniform1f(this->uSpotLight_Location.constant, spot_light.constant);
    glUniform1f(this->uSpotLight_Location.linear, spot_light.linear);
    glUniform1f(this->uSpotLight_Location.quadratic, spot_light.quadratic);

    glUniform3fv(this->uSpotLight_Location.ambient, 1, &spot_light.ambient.x);
    glUniform3fv(this->uSpotLight_Location.diffuse, 1, &spot_light.diffuse.x);
    glUniform3fv(this->uSpotLight_Location.specular, 1, &spot_light.specular.x);
}

void ShaderModel::setDirectionalLight(DirectionalLight &directional_light) const {
    this->use();

    glUniform3fv(this->uDirectionalLight_Location.direction, 1, &directional_light.direction.x);

    glUniform3fv(this->uDirectionalLight_Location.ambient, 1, &directional_light.ambient.x);
    glUniform3fv(this->uDirectionalLight_Location.diffuse, 1, &directional_light.diffuse.x);
    glUniform3fv(this->uDirectionalLight_Location.specular, 1, &directional_light.specular.x);
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

void ShaderModel::setUseFlashlight(bool useFlashlight) const {
    this->use();

    glUniform1i(this->uUseFlashlight_Location, (int)useFlashlight);
}
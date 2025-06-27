#ifndef PHONG_LIGHTING_H
#define PHONG_LIGHTING_H

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diffuse_light = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular_light = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 diffuse_texture = vec3(texture(material.diffuse, TextCoords));
    vec3 specular_texture = vec3(texture(material.specular, TextCoords));

    vec3 ambient = light.ambient * diffuse_texture;
    vec3 diffuse = light.diffuse * diffuse_light * diffuse_texture;
    vec3 specular = light.specular * specular_light * specular_texture;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    const float lightConstant = 1.0f;
    vec3 lightDir = normalize(light.position - FragPos);

    float diffuse_light = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular_light = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (lightConstant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 diffuse_texture = vec3(texture(material.diffuse, TextCoords));
    vec3 specular_texture = vec3(texture(material.specular, TextCoords));

    vec3 ambient = light.ambient * diffuse_texture;
    vec3 diffuse = light.diffuse * diffuse_light * diffuse_texture;
    vec3 specular = light.specular * specular_light * specular_texture;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
    const float lightConstant = 1.0f;
    vec3 lightDir = normalize(light.position - FragPos);

    float diffuse_light = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular_light = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (lightConstant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 diffuse_texture = vec3(texture(material.diffuse, TextCoords));
    vec3 specular_texture = vec3(texture(material.specular, TextCoords));

    vec3 ambient = light.ambient * diffuse_texture;
    vec3 diffuse = light.diffuse * diffuse_light * diffuse_texture;
    vec3 specular = light.specular * specular_light * specular_texture;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
#endif // PHONG_LIGHTING_H
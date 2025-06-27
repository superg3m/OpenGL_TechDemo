#ifndef PHONG_LIGHTING_H
#define PHONG_LIGHTING_H

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D normal_map;
    vec3      ambient_color;
    vec3      diffuse_color;
    vec3      specular_color;
    float     shininess;
    float     opacity;
};

uniform Material material; // Declare the material uniform

vec3 getDiffuseColor(vec2 texCoords) {
    vec3 color = texture(material.diffus_map, texCoords).rgb;
    // return pow(color, vec3(2.2)); This is if you want to apply gamma correction

    return color;
}

vec3 getSpecularColor(vec2 texCoords) {
    vec3 color = texture(material.specula_map, texCoords).rgb;
    return color;
}

vec3 getNormalFromMap(vec2 texCoords, vec3 N, mat3 TBN) {
    // return normalize(TBN * (texture(material.normalMap, texCoords).rgb * 2.0 - 1.0));
    return normalize(N);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 textCoords) {
    normal = normalize(normal);
    vec3 lightDir = normalize(-light.direction);

    float diffuse_factor = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float specular_light = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 diffuse_color = getDiffuseColor(textCoords);
    vec3 specular_color = getSpecularColor(textCoords);

    vec3 ambient = light.ambient * (material.ambient_color + diffuse_color);
    vec3 diffuse = light.diffuse * diffuse_factor * diffuse_color;
    vec3 specular = light.specular * specular_factor * specular_color;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 textCoords) {
    normal = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);

    float diffuse_factor = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float specular_light = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance)); // Removed 'f'

    vec3 diffuse_color = getDiffuseColor(textCoords);
    vec3 specular_color = getSpecularColor(textCoords);

    vec3 ambient = light.ambient * (material.ambient_color + diffuse_color);
    vec3 diffuse = light.diffuse * diffuse_factor * diffuse_color;
    vec3 specular = light.specular * specular_factor * specular_color;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 textCoords) {
    normal = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);

    float diffuse_factor = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float specular_light = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 diffuse_color = getDiffuseColor(textCoords);
    vec3 specular_color = getSpecularColor(textCoords);

    vec3 ambient = light.ambient * (material.ambient_color + diffuse_color);
    vec3 diffuse = light.diffuse * diffuse_factor * diffuse_color;
    vec3 specular = light.specular * specular_factor * specular_color;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
#endif // PHONG_LIGHTING_H
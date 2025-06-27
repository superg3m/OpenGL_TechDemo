// Best practice: define light structs BEFORE the Phong lighting header
// Assuming these are in another header file like "lights.glsl"
/*
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// ... similar for PointLight and SpotLight
*/

#ifndef PHONG_LIGHTING_H
#define PHONG_LIGHTING_H

// Assume TextCoords (vec2), FragPos (vec3), and CameraPos (vec3) are
// available as 'in' or 'uniform' variables in your main fragment shader.
// You might pass them into these functions from main.

struct Material {
    sampler2D diffuseMap;    // Diffuse color texture
    sampler2D specularMap;   // Specular intensity/color texture (often grayscale)
    sampler2D normalMap;     // For Normal Mapping (highly recommended for realism)
    vec3      ambientColor;  // Base ambient color for the material
    vec3      diffuseColor;  // Base diffuse color (used if no diffuseMap or as tint)
    vec3      specularColor; // Base specular color (used if no specularMap or as tint)
    float     shininess;     // Controls size/sharpness of specular highlight
    float     opacity;       // Controls transparency (alpha component)
};

uniform Material material; // Declare the material uniform

vec3 getDiffuseColor(vec2 texCoords) {
    vec3 color = texture(material.diffuseMap, texCoords).rgb;
    return color;
}

vec3 getSpecularColor(vec2 texCoords) {
    vec3 color = texture(material.specularMap, texCoords).rgb;
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
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular_factor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 diffuseColor = getDiffuseColor(textCoords);
    vec3 specularColor = getSpecularColor(textCoords);

    vec3 ambient = light.ambient * (material.ambientColor + diffuseColor);
    vec3 diffuse = light.diffuse * diffuse_factor * diffuseColor;
    vec3 specular = light.specular * specular_factor * specularColor;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 textCoords) {
    normal = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);

    float diffuse_factor = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular_factor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance)); // Removed 'f'

    vec3 diffuseColor = getDiffuseColor(textCoords);
    vec3 specularColor = getSpecularColor(textCoords);

    vec3 ambient = light.ambient * (material.ambientColor + diffuseColor);
    vec3 diffuse = light.diffuse * diffuse_factor * diffuseColor;
    vec3 specular = light.specular * specular_factor * specularColor;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 textCoords) {
    normal = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);

    float diffuse_factor = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular_factor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 diffuseColor = getDiffuseColor(textCoords);
    vec3 specularColor = getSpecularColor(textCoords);

    vec3 ambient = light.ambient * (material.ambientColor + diffuseColor);
    vec3 diffuse = light.diffuse * diffuse_factor * diffuseColor;
    vec3 specular = light.specular * specular_factor * specularColor;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
#endif // PHONG_LIGHTING_H
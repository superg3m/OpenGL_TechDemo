#version 330 core

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;        
};

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
    vec3        ambient_color;
    vec3        diffuse_color;
    vec3        specular_color;
    float       shininess;
    float       opacity;
};

#define NR_POINT_LIGHTS 4
uniform PointLight uPointLights[NR_POINT_LIGHTS];

uniform Material uMaterial;
uniform vec3 uViewPosition; 
uniform DirLight uDirLight;
uniform SpotLight uSpotLight;
uniform bool uUseFlashlight; 

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 FragColor;

vec3 CalcDirLight(const DirLight light, const vec3 normal, const vec3 viewDir, const vec2 texCoord);
vec3 CalcPointLight(const PointLight light, const vec3 normal, const vec3 fragPos, const vec3 viewDir, const vec2 texCoord);
vec3 CalcSpotLight(const SpotLight light, const vec3 normal, const vec3 fragPos, const vec3 viewDir, const vec2 texCoord);
vec3 getDiffuseColor(const vec2 texCoord);
vec3 getSpecularColor(const vec2 texCoord);

void main() {     
    // Properties (pre-calculate for efficiency, only normalize once)
    vec3 norm = normalize(v_Normal); // Normal is interpolated, so normalize it here
    vec3 viewDir = normalize(uViewPosition - v_FragPos);
    
    // Total lighting result
    vec3 totalLightResult = vec3(0.0f); 

    // Phase 1: directional lighting
    totalLightResult += CalcDirLight(uDirLight, norm, viewDir, v_TexCoord);

    // Phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        totalLightResult += CalcPointLight(uPointLights[i], norm, v_FragPos, viewDir, v_TexCoord);         
    }

    // Phase 3: spot light (flashlight)
    if (uUseFlashlight) {
        totalLightResult += CalcSpotLight(uSpotLight, norm, v_FragPos, viewDir, v_TexCoord);         
    }
    
    FragColor = vec4(totalLightResult, uMaterial.opacity);
}

// =================================================================================================
// Helper Functions (for material properties)
// =================================================================================================

vec3 getDiffuseColor(const vec2 texCoord) {
    return texture(uMaterial.diffuse_map, texCoord).rgb;
}

vec3 getSpecularColor(const vec2 texCoord) {
    return texture(uMaterial.specular_map, texCoord).rgb;
}


// =================================================================================================
// Light Calculation Functions
// =================================================================================================

vec3 CalcDirLight(const DirLight light, const vec3 normal, const vec3 viewDir, const vec2 texCoord) {
    vec3 lightDir = normalize(-light.direction); // Directional light direction is inverse of uniform value

    // Diffuse calculation
    float diffuse_factor = max(dot(normal, lightDir), 0.0f);

    // Specular calculation
    vec3 reflectDir = reflect(-lightDir, normal); // Light direction, not negative light direction
    float specular_factor = pow(max(dot(viewDir, reflectDir), 0.0f), uMaterial.shininess);

    // Get material colors
    vec3 diffuse_mat_color = getDiffuseColor(texCoord);
    vec3 specular_mat_color = getSpecularColor(texCoord);

    // Combine light and material components
    vec3 ambient = light.ambient * (uMaterial.ambient_color + diffuse_mat_color); // Ambient usually uses diffuse color as well
    vec3 diffuse = light.diffuse * diffuse_factor * diffuse_mat_color;
    vec3 specular = light.specular * specular_factor * specular_mat_color;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(const PointLight light, const vec3 normal, const vec3 fragPos, const vec3 viewDir, const vec2 texCoord) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse calculation
    float diffuse_factor = max(dot(normal, lightDir), 0.0f);

    // Specular calculation
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular_factor = pow(max(dot(viewDir, reflectDir), 0.0f), uMaterial.shininess);

    // Attenuation calculation (fixed float suffixes)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Get material colors
    vec3 diffuse_mat_color = getDiffuseColor(texCoord);
    vec3 specular_mat_color = getSpecularColor(texCoord);

    // Combine light and material components
    vec3 ambient = light.ambient * (uMaterial.ambient_color + diffuse_mat_color);
    vec3 diffuse = light.diffuse * diffuse_factor * diffuse_mat_color;
    vec3 specular = light.specular * specular_factor * specular_mat_color;

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(const SpotLight light, const vec3 normal, const vec3 fragPos, const vec3 viewDir, const vec2 texCoord) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse calculation
    float diffuse_factor = max(dot(normal, lightDir), 0.0f);

    // Specular calculation
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular_factor = pow(max(dot(viewDir, reflectDir), 0.0f), uMaterial.shininess);

    // Attenuation calculation (fixed float suffixes)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Spotlight effect
    float theta = dot(lightDir, normalize(-light.direction)); // Angle between fragment-to-light and spotlight direction
    float epsilon = light.cutOff - light.outerCutOff; // Blending range
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f); // Smooth step for falloff

    // Get material colors
    vec3 diffuse_mat_color = getDiffuseColor(texCoord);
    vec3 specular_mat_color = getSpecularColor(texCoord);

    // Combine light and material components
    vec3 ambient = light.ambient * (uMaterial.ambient_color + diffuse_mat_color);
    vec3 diffuse = light.diffuse * diffuse_factor * diffuse_mat_color;
    vec3 specular = light.specular * specular_factor * specular_mat_color;

    // Apply attenuation and spotlight intensity
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
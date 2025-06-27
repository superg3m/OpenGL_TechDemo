

#version 330 core
out vec4 FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

#include "../common/blinn_phong_lighting.glsl"

#define NR_POINT_LIGHTS 4
PointLight uPointLights[NR_POINT_LIGHTS];

void main() {    
    // properties

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(uViewPosition - FragPos);
    
    // phase 1: directional lighting
    vec3 result = CalcDirLight(uDirLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(uPointLights[i], norm, FragPos, viewDir);    
    }
    // phase 3: spot light
    if (uUseFlashlight) {
        result += CalcSpotLight(uSpotLight, norm, FragPos, viewDir);    
    }
    
    FragColor = vec4(result, uMaterial.opacity);
}
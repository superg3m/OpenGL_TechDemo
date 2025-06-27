

#version 330 core
out vec4 FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

#include "../common/blinn_phong_lighting.glsl"

void main() {           
    vec3 color = texture(uMaterial.diffuse, v_TexCoord).rgb;

    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 1; ++i) {
        lighting += CalcPointLight(normalize(v_Normal), v_FragPos, uLightPositions[i], uLightColors[i]);
    }

    color *= lighting;
    FragColor = vec4(color, 1.0);
    return;

    color *= lighting;
    if(uGamma) {
        color = pow(color, vec3(1.0/2.2));
    }
}
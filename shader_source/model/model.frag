

#version 330 core
out vec4 FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    vec4 color;
}; 

uniform Material uMaterial;
uniform vec3 uLightPositions[4];
uniform vec3 uLightColors[4];
uniform vec3 uViewPosition;
uniform bool uGamma;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor) {
    // diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(uViewPosition - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // simple attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (uGamma ? distance * distance : distance);
    
    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular;
}

void main() {           
    vec3 color = texture(uMaterial.diffuse, v_TexCoord).rgb;

    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 1; ++i) {
        lighting += BlinnPhong(normalize(v_Normal), v_FragPos, uLightPositions[i], uLightColors[i]);
    }

    color *= lighting;
    FragColor = vec4(color, 1.0);
    return;

    color *= lighting;
    if(uGamma) {
        color = pow(color, vec3(1.0/2.2));
    }
}
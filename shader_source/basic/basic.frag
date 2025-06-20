#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec4 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightPosition; 
uniform vec3 uViewPosition; 
uniform sampler2D uColorTexture;
uniform bool uHasColorTexture;

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;

    vec3 result = ((ambient + diffuse + specular) * uObjectColor.rgb);
    if (uHasColorTexture) {
        result *= texture(uColorTexture, TexCoords).rgb;
    }
    FragColor = vec4(result, 1.0);
} 
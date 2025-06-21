#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material uMaterial;
uniform Light uLight;

uniform vec3 uViewPosition; 
uniform sampler2D uColorTexture;
uniform bool uHasColorTexture;

out vec4 FragColor;

void main() {
    // ambient
    vec3 ambient = uLight.ambient * uMaterial.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = uLight.diffuse * (diff * uMaterial.diffuse);
    
    // specular
    vec3 viewDir = normalize(uViewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    vec3 specular = uLight.specular * (spec * uMaterial.specular);  

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
} 
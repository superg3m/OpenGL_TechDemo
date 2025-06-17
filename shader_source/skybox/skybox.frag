#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube texture6;
uniform samplerCube hasTexture6;

void main() {    
    FragColor = texture(texture6, TexCoords);
}
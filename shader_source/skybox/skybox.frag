#version 330 core
out vec4 FragColor;

in vec3 TexCoord;
uniform samplerCube uSkyboxTexture;

void main() {    
    FragColor = texture(uSkyboxTexture, TexCoord);
}
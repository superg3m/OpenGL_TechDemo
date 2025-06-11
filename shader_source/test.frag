#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 spriteColor;

void main() {    
    color = vec4(spriteColor, 1.0) * mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.2);
}  
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uColorTexture;
uniform vec4 uColor;

void main() {
    FragColor = texture(uColorTexture, TexCoords);
    FragColor *= uColor;
}

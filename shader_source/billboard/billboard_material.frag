#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform bool useTexture;

void main() {
    vec4 texColor = texture(uTexture, TexCoords);
    FragColor = useTexture ? texColor * uColor : uColor;
}
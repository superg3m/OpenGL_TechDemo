#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

#define TEXTURE_COLOR    0
#define TEXTURE_DECAL    1
#define TEXTURE_SPECULAR 2
#define TEXTURE_NORMAL   3
#define TEXTURE_HEIGHT   4
#define TEXTURE_ALPHA    5
#define TEXTURE_CUBEMAP  6
#define TEXTURE_MAX      7

uniform sampler2D texture0; // TEXTURE_COLOR
uniform sampler2D texture1; // TEXTURE_DECAL
uniform sampler2D texture2; // TEXTURE_SPECULAR
uniform sampler2D texture3; // TEXTURE_NORMAL
uniform sampler2D texture4; // TEXTURE_HEIGHT
uniform sampler2D texture5; // TEXTURE_ALPHA
uniform sampler2D texture6; // TEXTURE_CUBEMAP

uniform sampler2D hasTexture0; // TEXTURE_COLOR
uniform sampler2D hasTexture1; // TEXTURE_DECAL
uniform sampler2D hasTexture2; // TEXTURE_SPECULAR
uniform sampler2D hasTexture3; // TEXTURE_NORMAL
uniform sampler2D hasTexture4; // TEXTURE_HEIGHT
uniform sampler2D hasTexture5; // TEXTURE_ALPHA
uniform sampler2D hasTexture6; // TEXTURE_CUBEMAP

uniform vec4 color;

void main() {
    FragColor = texture(texture0, TexCoords);
    FragColor *= color;
}

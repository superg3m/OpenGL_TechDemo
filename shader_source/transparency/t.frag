out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D uTexture;
uniform float uOpacity;

void main() {             
    if (texture(uTexture, TexCoords).a < 0.1) {
        discard;
    }

    vec4 result = texture(uTexture, TexCoords);
    if (result.r > 0.5) { // Adjust this threshold based on your texture's alpha values
        result.a = uOpacity;
    }

    FragColor = vec4(result);
}
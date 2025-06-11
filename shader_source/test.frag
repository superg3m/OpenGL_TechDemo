#version 330 core
in vec2 TexCoords;
out vec4 color;

struct Material {
    sampler2D mainTexture;
    sampler2D decalTexture;
    bool hasDecal;
};

uniform vec3 spriteColor;
uniform Material material;

void main() {
    vec4 mainTex = texture(material.mainTexture, TexCoords);
    vec3 finalColor = mainTex.rgb;
    float finalAlpha = mainTex.a;

    finalColor *= spriteColor;
    
    if (material.hasDecal) {
        vec4 decalTex = texture(material.decalTexture, TexCoords);
        vec3 transparentColor = vec3(1.0, 1.0, 1.0); 
        float threshold = 0.01;
        float distToWhite = distance(decalTex.rgb, transparentColor);
        float blendFactor = step(threshold, distToWhite);
        finalColor = mix(finalColor, decalTex.rgb, blendFactor);
    }

    color = vec4(finalColor, finalAlpha);
}

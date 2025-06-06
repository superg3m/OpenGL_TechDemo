#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
    vec4 color;

    sampler2D texture_color;
    sampler2D texture_decal;

    sampler2D texture_specular;
    sampler2D texture_normal;
    sampler2D texture_height;
    sampler2D texture_alpha;

    bool useColor;
    bool useDecal;
    bool useSpecular;
    bool useNormal;
    bool useHeight;
    bool useAlpha;

    float shininess;
};

uniform Material material;

vec3 getNormal() {
    if (!material.useNormal)
        return normalize(Normal);

    vec3 tangentNormal = texture(material.texture_normal, TexCoords).xyz * 2.0 - 1.0;
    return normalize(tangentNormal);
}

vec2 parallaxTexCoords(vec2 texCoords) {
    if (!material.useHeight)
        return texCoords;

    float height = texture(material.texture_height, texCoords).r;
    float heightScale = 0.05;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec2 pOffset = viewDir.xy * (height * heightScale);
    return texCoords - pOffset;
}

void main() {
    vec2 coords = parallaxTexCoords(TexCoords);

    vec4 baseColor = material.useColor ? texture(material.texture_color, coords) : material.color;
    if (!material.useColor) {
        baseColor = material.color;
    }

    if (material.useDecal) {
        vec4 decalColor = texture(material.texture_decal, coords);
        baseColor = mix(baseColor, decalColor, decalColor.a);
    }

    if (material.useAlpha) {
        float alpha = texture(material.texture_alpha, coords).r;
        if (alpha < 0.1)
            discard;
        baseColor.a *= alpha;
    }

    vec3 norm = getNormal();

    vec3 ambient = 0.1 * baseColor.rgb;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * baseColor.rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = 0.0;
    if (material.useSpecular) {
        float specMap = texture(material.texture_specular, coords).r;
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * specMap;
    }

    vec3 result = ambient + diffuse + spec * vec3(1.0);

    FragColor = vec4(result, baseColor.a);
}
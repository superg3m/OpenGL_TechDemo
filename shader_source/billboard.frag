in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform sampler2D texture1;

void main() {
    vec4 texColor = texture(texture1, TexCoords);
    
    vec3 result = texColor.rgb;
    FragColor = vec4(result, texColor.a);
}
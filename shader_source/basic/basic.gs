#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float uTime;
uniform bool uGeometryShader;
uniform mat4 uView;
uniform mat4 uProjection;


in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_texCoords;

out vec3 g_FragPos;
out vec3 g_Normal;
out vec2 g_TexCoords;

vec3 explode(vec3 position, vec3 normal) {
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(uTime) + 1.0) / 2.0) * magnitude; 
    return position + direction; // Returns world-space exploded position
}

vec3 getNormal() {
    vec3 a = gs_in[0].FragPos - gs_in[1].FragPos;
    vec3 b = gs_in[2].FragPos - gs_in[1].FragPos;
    return normalize(cross(a, b));
}

void main() {
    vec3 faceNormal = getNormal();

    for (int i = 0; i < 3; i++) {
        if (uGeometryShader) {
            FragPos = explode(gs_in[i].FragPos, faceNormal);
            Normal = gs_in[i].Normal;
        } else {
            FragPos = gs_in[i].FragPos;
            Normal = gs_in[i].Normal;
        }

        TexCoords = gs_in[i].texCoords;

        gl_Position = uProjection * uView * vec4(FragPos, 1.0); 

        EmitVertex();
    }

    EndPrimitive();
}
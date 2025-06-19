#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 mvp;

void main() {
    mat4 billboardModel = model;
    // Overwrite rotation part to face camera (remove rotation)
    billboardModel[0][0] = 1.0;
    billboardModel[0][1] = 0.0;
    billboardModel[0][2] = 0.0;
    billboardModel[1][0] = 0.0;
    billboardModel[1][1] = 1.0;
    billboardModel[1][2] = 0.0;
    billboardModel[2][0] = 0.0;
    billboardModel[2][1] = 0.0;
    billboardModel[2][2] = 1.0;

    TexCoords = aTexCoords;
    gl_Position = mvp * billboardModel * vec4(aPos, 1.0);
}
#pragma once

#include <ckit.h>
#include <GLAD/glad.h>
#include <shader.h>

typedef struct VertexBuffer {
    float* vertices;
    u32* vertex_component;
    u32 vertex_count;
    u32 vertex_component_count;
    u32 stride;
} VertexBuffer;

typedef struct Mesh {
    CKIT_Vector3 position;
    VertexBuffer vertex_buffer;
    u32* indices;
    Shader* shader;
    u32 indices_count;
    u32 VBO, VAO, EBO;
    GLenum draw_mode;
} Mesh;

VertexBuffer vertex_buffer_create(float* vertices, u32 vertex_count, u32* vertex_component, u32 vertex_component_count);

Mesh mesh_create(CKIT_Vector3 position, Shader* shader, VertexBuffer vertex_buffer, u32* indices, u32 indices_count, GLenum draw_mode);
void mesh_draw(Mesh* mesh);
void mesh_set_position(Mesh* mesh, CKIT_Vector3 position);
void mesh_set_vertices(Mesh* mesh, VertexBuffer vertex_buffer);
void mesh_set_indices(Mesh* mesh, u32* indices, u32 indices_count);
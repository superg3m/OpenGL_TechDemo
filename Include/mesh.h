#pragma once

#include <ckit.h>
#include <GLAD/glad.h>
#include <shader.h>

// Date: October 14, 2024
// TODO(Jovanni): 
    // I don't think I like this...
    // I actually think that the shader should know the Number of VertexShaderAttributes
    // This should be simple to implement but easy to fuck since I would use a index of which means the substring has to be correct.

typedef struct Mesh {
    CKIT_Vector3 position;
    // Transform* transform; This will eventually be a transform
    float* vertices;
    u32* indices;
    Shader* shader; // just a pointer to the shader we might want to make a copy actually because what if the shader goes out of scope?
} Mesh;

typedef struct RenderGroup {
    Mesh** meshes; // After you add a single mesh you can make sure the previuos mesh has the same values for the VertexAttributes
    float* vertices;
    u32* indices;
    // u32* vertex_offsets;
    // u32* index_offsets;
    u32* attributes;

    u32* mesh_vertex_counts;
    u32* mesh_index_counts;
    u32 stride;
    u32 VBO, VAO, EBO;
    GLenum draw_mode; // GL_TRIANGLES
    GLenum binded_draw_state; // GL_STATIC_DRAW; // GL_STATIC_DRAW
} RenderGroup;

RenderGroup render_group_create(GLenum draw_mode, GLenum binded_draw_state);
void render_group_free(RenderGroup* render_group);
void render_group_add(RenderGroup* render_group, Mesh* mesh);
void render_group_finalize(RenderGroup* render_group);
void render_group_draw(RenderGroup* render_group);

Mesh mesh_create(CKIT_Vector3 position, Shader* shader, float* vertices, u32* indices);
/*
void mesh_draw(Mesh* mesh);
void mesh_set_position(Mesh* mesh, CKIT_Vector3 position);
void mesh_set_vertices(Mesh* mesh, VertexBuffer vertex_buffer);
void mesh_set_indices(Mesh* mesh, u32* indices, u32 indices_count);
*/
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
    GLenum binded_draw_state; // GL_STATIC_DRAW; // GL_STATIC_DRAW
    u32 VAO;
    u32 VBO; 
    u32 EBO;
} Mesh;

typedef struct RenderGroup {
    Mesh** meshes; // After you add a single mesh you can make sure the previuos mesh has the same values for the VertexAttributes
    GLenum draw_mode; // GL_TRIANGLES
} RenderGroup;

RenderGroup render_group_create(GLenum draw_mode);
void render_group_free(RenderGroup* render_group);
void render_group_add(RenderGroup* render_group, Mesh* mesh);
void render_group_finalize(RenderGroup* render_group);
void render_group_draw(RenderGroup* render_group);

Mesh mesh_create(CKIT_Vector3 position, Shader* shader, float* vertices, u32* indices, GLenum binded_draw_state);
void mesh_draw(Mesh* mesh, GLenum binded_draw_state);
void mesh_set_position(Mesh* mesh, CKIT_Vector3 position);
void mesh_set_vertices(Mesh* mesh, float* vertices);
void mesh_set_indices(Mesh* mesh, u32* indices);
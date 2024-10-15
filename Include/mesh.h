#pragma once

#include <ckit.h>
#include <GLAD/glad.h>
#include <shader.h>

// Date: October 14, 2024
// TODO(Jovanni): 
    // I don't think I like this...
    // I actually think that the shader should know the Number of VertexShaderAttributes
    // This should be simple to implement but easy to fuck since I would use a index of which means the substring has to be correct.
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

typedef struct NewMesh {
    CKIT_Vector3 position;
    // Transform* transform; This will eventually be a transform
    u32* vertices;
    u32* indices;
    u32 vertices_count;
    u32 indices_count;
    Shader* shader;
} NewMesh;

typedef struct RenderGroup {
    NewMesh* meshes; // After you add a single mesh you can make sure the previuos mesh has the same values for the VertexAttributes
    u32* vertex_offset;
    u32* index_offset;

    u32 VBO, VAO, EBO;
    GLenum draw_mode;
} RenderGroup;

VertexBuffer vertex_buffer_create(float* vertices, u32 vertex_count, u32* vertex_component, u32 vertex_component_count);

RenderGroup render_group_create(GLenum draw_mode);
void render_group_add_mesh(RenderGroup* render_group, NewMesh* mesh);
void render_group_finalize(RenderGroup* render_group);

Mesh mesh_create(CKIT_Vector3 position, Shader* shader, VertexBuffer vertex_buffer, u32* indices, u32 indices_count, GLenum draw_mode);
void mesh_draw(Mesh* mesh);
void mesh_set_position(Mesh* mesh, CKIT_Vector3 position);
void mesh_set_vertices(Mesh* mesh, VertexBuffer vertex_buffer);
void mesh_set_indices(Mesh* mesh, u32* indices, u32 indices_count);
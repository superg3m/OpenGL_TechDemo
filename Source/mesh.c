#include <mesh.h>

VertexBuffer vertex_buffer_create(float* vertices, u32 vertex_count, u32* vertex_component, u32 vertex_component_count) {
    VertexBuffer ret; 
    ret.vertices = vertices;
    ret.vertex_count = vertex_count;
    ret.vertex_component = vertex_component;
    ret.vertex_component_count = vertex_component_count;

    u32 sum = 0; 
    for (int i = 0; i < ret.vertex_component_count; i++) {
        u32 num_components = ret.vertex_component[i];
        ckit_assert_msg((num_components > 0 && num_components <= 4), "Got: %d vertex components must be between (0 - 4) \n", num_components);
        sum += num_components;
    }

    ret.stride = sum;

    return ret;
}

Mesh mesh_create(CKIT_Vector3 position, Shader* shader, VertexBuffer vertex_buffer, u32* indices, u32 indices_count, GLenum draw_mode) {
    Mesh ret;
    ret.position = position;
    ret.vertex_buffer = vertex_buffer;
    ret.indices = indices;
    ret.indices_count = indices_count;
    ret.draw_mode = draw_mode;
    ret.shader = shader;
    ret.VAO = -1;
    ret.VBO = -1;
    ret.EBO = -1;

    glGenVertexArrays(1, &ret.VAO);
    glGenBuffers(1, &ret.VBO);
    glGenBuffers(1, &ret.EBO);
    glBindVertexArray(ret.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, ret.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ret.vertex_buffer.vertex_count, ret.vertex_buffer.vertices, draw_mode);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indices_count, ret.indices, draw_mode);

    u64 vertex_component_offset = 0;
    for (int i = 0; i < vertex_buffer.vertex_component_count; i++) {
        u32 num_components = vertex_buffer.vertex_component[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, num_components, GL_FLOAT, GL_FALSE, ret.vertex_buffer.stride * sizeof(float), (void*)(vertex_component_offset * sizeof(float)));
        vertex_component_offset += num_components;
    }    

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    return ret;
}

void mesh_draw(Mesh* mesh) {
    shader_use(mesh->shader);
    shader_bind_textures(mesh->shader);

    glBindVertexArray(mesh->VAO);

    if (mesh->indices_count != 0) {
        glDrawElements(GL_TRIANGLES, mesh->vertex_buffer.vertex_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_buffer.vertex_count);
    }

    shader_unbind_textures(mesh->shader);
}

void mesh_set_position(Mesh* mesh, CKIT_Vector3 position) {
    mesh->position = position;
}
void mesh_set_vertices(Mesh* mesh, VertexBuffer vertex_buffer) {
    ckit_assert_msg(mesh->draw_mode != GL_STATIC_DRAW, "You may not modify a mesh that is marked as GL_STATIC_DRAW\n");

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_buffer.vertex_count * sizeof(float), vertex_buffer.vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    mesh->vertex_buffer = vertex_buffer;
}
void mesh_set_indices(Mesh* mesh, u32* indices, u32 indices_count) {
    ckit_assert_msg(mesh->draw_mode != GL_STATIC_DRAW, "You may not modify a mesh that is marked as GL_STATIC_DRAW\n");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices_count * sizeof(GLuint), indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh->indices = indices;
    mesh->indices_count = indices_count;
}
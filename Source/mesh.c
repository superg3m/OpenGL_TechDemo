#include <mesh.h>

RenderGroup render_group_create(GLenum draw_mode, GLenum binded_draw_state) {
    RenderGroup ret;
    ret.draw_mode = draw_mode;
    ret.binded_draw_state = binded_draw_state;
    ret.meshes = ckit_vector_reserve(1, Mesh*);
    ret.vertices = ckit_vector_reserve(1, float);
    ret.indices = ckit_vector_reserve(1, u32);
    // ret.vertex_offsets = ckit_vector_reserve(1, u32);
    // ret.index_offsets = ckit_vector_reserve(1, u32);
    ret.attributes = ckit_vector_reserve(1, u32);
    ret.mesh_vertex_counts = NULL;
    ret.mesh_index_counts = NULL;
    ret.VAO = -1;
    ret.VBO = -1;
    ret.EBO = -1;

    return ret;
}

void render_group_free(RenderGroup* render_group) {
    ckit_vector_free(render_group->meshes);
    ckit_vector_free(render_group->vertices);
    ckit_vector_free(render_group->indices);
    // ckit_vector_free(render_group->vertex_offsets);
    // ckit_vector_free(render_group->index_offsets);
    ckit_vector_free(render_group->attributes);
    ckit_vector_free(render_group->mesh_vertex_counts);
    ckit_vector_free(render_group->mesh_index_counts);
}

void render_group_add(RenderGroup* render_group, Mesh* mesh) {
    if (ckit_vector_count(render_group->meshes)) {
        u32 last_index = ckit_vector_count(render_group->meshes) - 1;
        u32* previous_mesh_attributes = render_group->meshes[last_index]->shader->attributes;

        ckit_assert(ckit_vector_count(previous_mesh_attributes) == ckit_vector_count(mesh->shader->attributes));

        u32 sum_of_attribute = 0;
        for (int i = 0; i < ckit_vector_count(previous_mesh_attributes); i++) {
            ckit_assert(mesh->shader->attributes[i] == previous_mesh_attributes[i]);
            sum_of_attribute += previous_mesh_attributes[i];
        }

        ckit_assert(render_group->stride == sum_of_attribute); 
        render_group->stride = sum_of_attribute;

        // u32 previous_vertex_offset = render_group->vertex_offsets[last_index];
        // u32 previous_index_offset = render_group->index_offsets[last_index];
        // ckit_vector_push(render_group->vertex_offsets, previous_vertex_offset + (ckit_vector_count(mesh->vertices) * sizeof(float)));
        // ckit_vector_push(render_group->index_offsets, previous_index_offset + (ckit_vector_count(mesh->indices) * sizeof(u32)));
    } else {
        // ckit_vector_push(render_group->vertex_offsets, 0 * sizeof(float));
        // ckit_vector_push(render_group->index_offsets, 0 * sizeof(u32));

        u32 sum_of_attribute = 0;
        for (int i = 0; i < ckit_vector_count(mesh->shader->attributes); i++) {
            sum_of_attribute += mesh->shader->attributes[i];
        }
        render_group->stride = sum_of_attribute;
    }

    for (int i = 0; i < ckit_vector_count(mesh->vertices); i++) {
        ckit_vector_push(render_group->vertices, mesh->vertices[i]);
    }

    for (int i = 0; i < ckit_vector_count(mesh->indices); i++) {
        ckit_vector_push(render_group->indices, mesh->indices[i]);
    }

    ckit_vector_push(render_group->mesh_vertex_counts, ckit_vector_count(mesh->vertices));
    ckit_vector_push(render_group->mesh_index_counts, ckit_vector_count(mesh->indices));
    ckit_vector_push(render_group->meshes, mesh);
}

void render_group_finalize(RenderGroup* render_group) {
    ckit_assert_msg(ckit_vector_count(render_group->vertices), "You may not finalize a render group that has no meshes added!\n");

    glGenVertexArrays(1, &render_group->VAO);
    glGenBuffers(1, &render_group->VBO);
    glGenBuffers(1, &render_group->EBO);
    glBindVertexArray(render_group->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, render_group->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ckit_vector_count(render_group->vertices), render_group->vertices, render_group->binded_draw_state);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_group->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * ckit_vector_count(render_group->indices), render_group->indices, render_group->binded_draw_state);

    u64 vertex_component_offset = 0;
    for (int i = 0; i < ckit_vector_count(render_group->meshes[0]->shader->attributes); i++) {
        u32 num_components = render_group->meshes[0]->shader->attributes[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, num_components, GL_FLOAT, GL_FALSE, render_group->stride * sizeof(float), (void*)(vertex_component_offset * sizeof(float)));
        vertex_component_offset += num_components;
    }    

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void render_group_draw(RenderGroup* render_group) {
    ckit_assert_msg(render_group->VBO != -1, "Must finalize render group before drawing it\n");
    ckit_assert_msg(render_group->VAO != -1, "VAO is not valid\n");

    glBindVertexArray(render_group->VAO);

    if (render_group->meshes[0]->indices) {
        GLsizei* index_counts = render_group->mesh_index_counts;
        void* indices = render_group->indices;
        GLsizei count = ckit_vector_count(render_group->meshes);

        glMultiDrawElements(render_group->draw_mode, index_counts, GL_UNSIGNED_INT, &indices, count);

        // Check for OpenGL errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            LOG_FATAL("OPENGL_ERROR: %d\n", error);
        }
    } else {
        // Handle the case where there are no indices
    }

    glBindVertexArray(0);
}

Mesh mesh_create(CKIT_Vector3 position, Shader* shader, float* vertices, u32* indices) {
    Mesh ret;
    ret.position = position;
    ret.vertices = vertices;
    ret.indices = indices;
    ret.shader = shader;

    return ret;
}

/*
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
*/
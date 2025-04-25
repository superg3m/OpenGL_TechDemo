#include <mesh.h>

RenderGroup render_group_create(GLenum draw_mode) {
    RenderGroup ret;
    ret.draw_mode = draw_mode;
    ret.meshes = ckit_vector_reserve(1, Mesh*);

    return ret;
}

void render_group_free(RenderGroup* render_group) {
    ckit_vector_free(render_group->meshes);
}

internal Boolean mesh_attributes_are_similar(Mesh* previous_mesh, Mesh* new_mesh) {
    if (!previous_mesh) {
        return TRUE;
    }

    ckit_assert(ckit_vector_count(previous_mesh->shader->attributes) == ckit_vector_count(new_mesh->shader->attributes));

    u32 sum = 0;
    for (u32 i = 0; i < ckit_vector_count(previous_mesh->shader->attributes); i++) {
        sum += new_mesh->shader->attributes[i];
        if (previous_mesh->shader->attributes[i] != new_mesh->shader->attributes[i]) {
            return FALSE;
        }
    }

    return TRUE;
}

void render_group_add(RenderGroup* render_group, Mesh* mesh) {
    Mesh* previous_mesh = ckit_vector_count(render_group->meshes) != 0 ?  render_group->meshes[ckit_vector_count(render_group->meshes)] : NULLPTR;
    ckit_assert(mesh_attributes_are_similar(previous_mesh, mesh));

    ckit_vector_push(render_group->meshes, mesh);
}

void mesh_draw(Mesh* mesh, GLenum draw_mode) {
    shader_use(mesh->shader);
    shader_bind_textures(mesh->shader);

    glBindVertexArray(mesh->VAO);

    if (ckit_vector_count(mesh->indices) != 0) {
        GLsizei index_count = ckit_vector_count(mesh->indices);
        glDrawElements(draw_mode, index_count, GL_UNSIGNED_INT, (void*)0);
    } else {
        glDrawArrays(draw_mode, 0, ckit_vector_count(mesh->vertices));
    }

    shader_unbind_textures(mesh->shader);

    glBindVertexArray(0);
}

void render_group_draw(RenderGroup* render_group) {
    for (u32 i = 0; i < ckit_vector_count(render_group->meshes); i++) {
        Mesh* current_mesh = render_group->meshes[i];

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, current_mesh->position);
        transform = glm::scale(transform, current_mesh->scale);
        transform = glm::rotate(transform, current_mesh->rotation_speed, current_mesh->rotation);

        shader_set_mat4(current_mesh->shader, "transform", transform);

        mesh_draw(current_mesh, render_group->draw_mode);
    }
}

Mesh mesh_create(Shader* shader, float* vertices, u32* indices, GLenum binded_draw_state) {
    Mesh ret;
    ret.position = glm::vec3(0.0f);
    ret.rotation = glm::vec3(0.0f);
    ret.scale = glm::vec3(1.0f);
    ret.rotation_speed = 0.0f;

    ret.vertices = vertices;
    ret.indices = indices;
    ret.shader = shader;
    ret.binded_draw_state = binded_draw_state;
    ret.VAO = -1;
    ret.VBO = -1;
    ret.EBO = -1;

    glGenVertexArrays(1, &ret.VAO);
    glGenBuffers(1, &ret.VBO);
    glGenBuffers(1, &ret.EBO);
    glBindVertexArray(ret.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, ret.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ckit_vector_count(ret.vertices), ret.vertices, binded_draw_state);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * ckit_vector_count(ret.indices), ret.indices, binded_draw_state);

    u32 stride = 0;
    for (u32 i = 0; i < ckit_vector_count(shader->attributes); i++) {
        stride += shader->attributes[i];
    }

    u64 vertex_component_offset = 0;
    for (u32 i = 0; i < ckit_vector_count(shader->attributes); i++) {
        u32 num_components = shader->attributes[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, num_components, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(vertex_component_offset * sizeof(float)));
        vertex_component_offset += num_components;
    }    

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    return ret;
}

void mesh_set_transform(Mesh* mesh, glm::vec3 position, float rotation_speed, glm::vec3 rotation, glm::vec3 scale) {
    mesh->position = position;
    mesh->rotation_speed = rotation_speed;
    mesh->rotation = rotation;
    mesh->scale = scale;
}

void mesh_set_position(Mesh* mesh, glm::vec3 position) {
    mesh->position = position;
}

void mesh_set_rotation(Mesh* mesh, float rotation_speed, glm::vec3 rotation) {
    mesh->rotation = rotation;
    mesh->rotation_speed = rotation_speed;
}

void mesh_set_scale(Mesh* mesh, glm::vec3 scale) {
    mesh->scale = scale;
}

void mesh_translate(Mesh* mesh, glm::vec3 translation) {
    mesh->position += translation;
}

void mesh_set_vertices(Mesh* mesh, float* vertices) {
    ckit_assert_msg(mesh->binded_draw_state != GL_STATIC_DRAW, "You may not modify a mesh that is marked as GL_STATIC_DRAW\n");

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ckit_vector_count(vertices) * sizeof(float), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    mesh->vertices = vertices;
}

void mesh_set_indices(Mesh* mesh, u32* indices) {
    ckit_assert_msg(mesh->binded_draw_state != GL_STATIC_DRAW, "You may not modify a mesh that is marked as GL_STATIC_DRAW\n");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ckit_vector_count(indices) * sizeof(u32), indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh->indices = indices;
}

#include <ckg.h>
#include <mesh.hpp>
#include <glad/glad.h>

Mesh::Mesh(Material material, Geometry geometry) {
    this->material = material;
    this->geometry = geometry;
}

void Mesh::draw(GM_Matrix4 transform) {
    this->material.shader.setMat4("model", transform);
    this->material.bindTextures();

    if (this->material.textures[TEXTURE_CUBEMAP] != TEXTURE_INVALID) {
        glDepthFunc(GL_LEQUAL);
    }

    glBindVertexArray(geometry.VAO);
    int index_count = geometry.indices.size();
    if (index_count > 0) {
        glDrawElements(this->geometry.draw_type, index_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(this->geometry.draw_type, 0, geometry.vertices.size());
    }

    if (this->material.textures[TEXTURE_CUBEMAP] != TEXTURE_INVALID) {
        glDepthFunc(GL_LESS);
    }

    this->material.unbindTextures();
    glBindVertexArray(0);
}
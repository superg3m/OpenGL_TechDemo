#include <ckg.h>
#include <Mesh.hpp>

Mesh::Mesh(Geometry geometry, Material material) {
    this->material = material;
    this->geometry = geometry;
}

void Mesh::draw(Shader &shader) {
    for (const auto &[key, id] : this->material.textures) {
        shader.bindTexture(key.c_str(), id);
    }

    glBindVertexArray(geometry.VAO);
    if (geometry.index_count > 0) {
        glDrawElements(this->geometry.draw_type, geometry.index_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(this->geometry.draw_type, 0, geometry.vertex_count);
    }

    glBindVertexArray(0);

    shader.unbindTextures();
}
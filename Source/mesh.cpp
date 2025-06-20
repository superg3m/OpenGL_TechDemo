#include <ckg.h>
#include <mesh.hpp>
#include <glad/glad.h>

Mesh::Mesh(Material material, Geometry geometry) {
    this->material = material;
    this->geometry = geometry;
}

void Mesh::draw() {
    glBindVertexArray(geometry.VAO);
    int index_count = geometry.indices.size();
    if (index_count > 0) {
        glDrawElements(this->geometry.draw_type, index_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(this->geometry.draw_type, 0, geometry.vertices.size());
    }
    glBindVertexArray(0);
}
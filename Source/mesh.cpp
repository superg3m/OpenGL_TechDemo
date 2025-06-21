#include <ckg.h>
#include <Mesh.hpp>

Mesh::Mesh(Geometry geometry, Material material) {
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
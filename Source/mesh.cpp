#include <ckg.h>
#include <mesh.hpp>
#include <glad/glad.h>

Mesh::Mesh(Material material, Geometry geometry, GLenum draw_type) {
    this->material = material;
    this->geometry = geometry;
    this->draw_type = draw_type;
}

void Mesh::setVertices(const std::vector<Vertex>& vertices) {
    ckg_assert(this->geometry.usage == GL_DYNAMIC_DRAW);

    geometry.vertices = vertices;

    glBindBuffer(GL_ARRAY_BUFFER, geometry.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setIndices(const std::vector<unsigned int>& indices) {
    ckg_assert(this->geometry.usage == GL_DYNAMIC_DRAW);
    geometry.indices = indices;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(GM_Matrix4 transform) {
    this->material.shader->setMat4("model", transform);
    this->material.bindTextures();

    glBindVertexArray(geometry.VAO);
    int index_count = geometry.indices.size();
    if (index_count > 0) {
        glDrawElements(draw_type, index_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(draw_type, 0, (geometry.vertices.size() / sizeof(Vertex)));
    }

    this->material.unbindTextures();
    glBindVertexArray(0);
}
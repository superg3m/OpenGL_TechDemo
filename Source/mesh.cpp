#include <mesh.hpp>
#include <glad/glad.h>

void Geometry::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), usage);

    if (!indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);
    }

    stride = 0;
    for (auto a : attributes) {
        stride += a;
    }

    int offset = 0;
    for (unsigned int i = 0; i < attributes.size(); ++i) {
        unsigned int size = attributes[i];
        if (size == 0) continue;

        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);

        offset += size;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Geometry::Geometry(const std::vector<unsigned int>& attributes, const std::vector<float>& vertices,  const std::vector<unsigned int>& indices, GLenum usage) {
    this->attributes = attributes;
    this->vertices = vertices;
    this->indices = indices;
    this->usage = usage;
    this->stride = 0;
    
    this->setup();
}

Material::Material(Shader* shader) : shader(shader) {}

Mesh::Mesh(Material material, Geometry geometry, GM_Matrix4 transform, GLenum draw_type) {
    this->material = material;
    this->geometry = geometry;
    this->transform = transform;
    this->draw_type = draw_type;
}

void Mesh::setVertices(const std::vector<float>& vertices) {
    geometry.vertices = vertices;

    glBindBuffer(GL_ARRAY_BUFFER, geometry.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setIndices(const std::vector<unsigned int>& indices) {
    geometry.indices = indices;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw() {
    material.shader->setMat4("model", transform);
    material.shader->bindTextures();

    glBindVertexArray(geometry.VAO);
    GLsizei index_count = static_cast<GLsizei>(geometry.indices.size());
    if (index_count > 0) {
        glDrawElements(draw_type, index_count, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(draw_type, 0, static_cast<GLsizei>(geometry.vertices.size() / geometry.stride));
    }

    material.shader->unbindTextures();
    glBindVertexArray(0);
}

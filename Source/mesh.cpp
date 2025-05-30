#include <mesh.hpp>
#include <glad/glad.h>

Mesh::Mesh(Material material, Geometry geometry, GM_Matrix4 transform) {
    this->material = material;
    this->geometry = geometry;
    this->transform = transform;
}

Material::Material(Shader* shader) {
    this->shader = shader;
}

void Mesh::draw() {
    this->material.shader->setMat4("model", this->transform);
    this->material.shader->bindTextures();

    glBindVertexArray(this->geometry.VAO);
    GLsizei index_count = this->geometry.indicies.size();
    if (index_count != 0) {
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (void*)0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    this->material.shader->unbindTextures();
    glBindVertexArray(0);
}

void Geometry::setup() {
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);

    if (this->indicies.size() != 0) {
        glGenBuffers(1, &this->EBO);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicies.size() * sizeof(unsigned int), &this->indicies[0], GL_STATIC_DRAW);
    }

    for (int i = 0; i < this->attributes.size(); i++) {
        this->stride += this->attributes[i];
    }

    int attribute_index = 0;
    int vertex_attribute_offset = 0;
    for (u32 i = 0; i <  this->attributes.size(); i++) {
        unsigned int attribute = this->attributes[i];
        
        if (attribute == 0) {
            continue;
        }

        glEnableVertexAttribArray(attribute_index);
        glVertexAttribPointer(attribute_index, attribute, GL_FLOAT, GL_FALSE, this->stride * sizeof(float), (void*)(vertex_attribute_offset * sizeof(float)));
        vertex_attribute_offset += attribute;

        attribute_index++;
    }    

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

Geometry::Geometry(std::vector<unsigned int> attributes, std::vector<float>& vertices, std::vector<unsigned int> indicies) {
    this->attributes = attributes;
    this->vertices = vertices;
    this->indicies = indicies;
    this->stride = 0;

    this->setup();
}
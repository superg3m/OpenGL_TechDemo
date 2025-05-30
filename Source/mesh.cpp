#include <mesh.hpp>
#include <glad/glad.h>

void setup_geometry(Geometry* geometry) {
    // Create buffers/arrays
	glGenVertexArrays(1, &geometry->VAO);
	glGenBuffers(1, &geometry->VBO);
	glGenBuffers(1, &geometry->EBO);

	glBindVertexArray(geometry->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->VBO);
	glBufferData(GL_ARRAY_BUFFER, geometry->verticies.size() * sizeof(Vertex), &geometry->verticies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indicies.size() * sizeof(unsigned int), &geometry->indicies[0], GL_STATIC_DRAW);

    int stride = 0;
    for (int i = 0; i < geometry->attributes.size(); i++) {
        stride += geometry->attributes[i];
    }

    int attribute_index = 0;
    int vertex_attribute_offset = 0;
    for (u32 i = 0; i <  geometry->attributes.size(); i++) {
        unsigned int attribute = geometry->attributes[i];
        
        if (attribute == 0) {
            continue;
        }

        glEnableVertexAttribArray(attribute_index);
        glVertexAttribPointer(attribute_index, attribute, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(vertex_attribute_offset * sizeof(float)));
        vertex_attribute_offset += attribute;

        attribute_index++;
    }    

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

Geometry Geometry::Cube(std::vector<unsigned int> attributes = {3, 0, 3, 2}) {

}

Mesh::Mesh(Material material, Geometry geometry, GM_Matrix4 transform = gm_mat4_identity()) {
    this->material = material;
    this->geometry = geometry;
    this->transform = transform;
}
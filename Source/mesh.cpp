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

    int attribute_index = 0;
    for (int i = 0; i < geometry->attributes.size(); i++) {
        unsigned int attribute = geometry->attributes[i];
        
        if (attribute == 0) {
            continue;
        }

        glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

        attribute_index++;
    }

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

	
	// COLOR STUFF AT 3 if you want

	/*

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Color));
	 
	// Vertex Tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));

	// Vertex Bitangent
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));

	// Bone IDs
	glEnableVertexAttribArray(6);
	glVertexAttribIPointer(6, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));

	// Weights
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_Weights));
	*/
	glBindVertexArray(0);
}

Geometry Geometry::Cube(std::vector<unsigned int> attributes = {3, 0, 3, 2}) {

}

Mesh::Mesh(Material material, Geometry geometry, GM_Matrix4 transform = gm_mat4_identity()) {
    this->material = material;
    this->geometry = geometry;
    this->transform = transform;
}
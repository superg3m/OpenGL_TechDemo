#include <geometry.hpp>

Geometry Quad() {
    std::vector<Vertex> quad_vertices = {
        //         Position         Normal     UV
        Vertex{-0.5f, -0.5f, 0.0f,  0, 0, 1,  0, 0}, // Bottom-left
        Vertex{ 0.5f, -0.5f, 0.0f,  0, 0, 1,  1, 0}, // Bottom-right
        Vertex{ 0.5f,  0.5f, 0.0f,  0, 0, 1,  1, 1}, // Top-right
        Vertex{-0.5f,  0.5f, 0.0f,  0, 0, 1,  0, 1}  // Top-left
    };

    std::vector<unsigned int> quad_indices = {
        0, 1, 2,
        2, 3, 0
    };

    Geometry ret;
    ret.vertices = quad_vertices;
    ret.indices = quad_indices;
    ret.usage = GL_STATIC_DRAW;
    ret.setup();

    return ret;
}


Geometry Cube() {
    std::vector<Vertex> cube_vertices = {
        // Front face
        Vertex{-0.5f, -0.5f, -0.5f,  0, 0, -1,  0, 0}, // 0
        Vertex{ 0.5f, -0.5f, -0.5f,  0, 0, -1,  1, 0}, // 1
        Vertex{ 0.5f,  0.5f, -0.5f,  0, 0, -1,  1, 1}, // 2
        Vertex{-0.5f,  0.5f, -0.5f,  0, 0, -1,  0, 1}, // 3

        // Back face
        Vertex{-0.5f, -0.5f, 0.5f,   0, 0, 1,   0, 0}, // 4
        Vertex{ 0.5f, -0.5f, 0.5f,   0, 0, 1,   1, 0}, // 5
        Vertex{ 0.5f,  0.5f, 0.5f,   0, 0, 1,   1, 1}, // 6
        Vertex{-0.5f,  0.5f, 0.5f,   0, 0, 1,   0, 1}, // 7

        // Left face
        Vertex{-0.5f, -0.5f,  0.5f, -1, 0, 0,   0, 0}, // 8
        Vertex{-0.5f, -0.5f, -0.5f, -1, 0, 0,   1, 0}, // 9
        Vertex{-0.5f,  0.5f, -0.5f, -1, 0, 0,   1, 1}, // 10
        Vertex{-0.5f,  0.5f,  0.5f, -1, 0, 0,   0, 1}, // 11

        // Right face
        Vertex{ 0.5f, -0.5f, -0.5f,  1, 0, 0,   0, 0}, // 12
        Vertex{ 0.5f, -0.5f,  0.5f,  1, 0, 0,   1, 0}, // 13
        Vertex{ 0.5f,  0.5f,  0.5f,  1, 0, 0,   1, 1}, // 14
        Vertex{ 0.5f,  0.5f, -0.5f,  1, 0, 0,   0, 1}, // 15

        // Bottom face
        Vertex{-0.5f, -0.5f, -0.5f,  0, -1, 0,  0, 1}, // 16
        Vertex{ 0.5f, -0.5f, -0.5f,  0, -1, 0,  1, 1}, // 17
        Vertex{ 0.5f, -0.5f,  0.5f,  0, -1, 0,  1, 0}, // 18
        Vertex{-0.5f, -0.5f,  0.5f,  0, -1, 0,  0, 0}, // 19

        // Top face
        Vertex{-0.5f,  0.5f, -0.5f,  0, 1, 0,   0, 1}, // 20
        Vertex{ 0.5f,  0.5f, -0.5f,  0, 1, 0,   1, 1}, // 21
        Vertex{ 0.5f,  0.5f,  0.5f,  0, 1, 0,   1, 0}, // 22
        Vertex{-0.5f,  0.5f,  0.5f,  0, 1, 0,   0, 0}, // 23
    };

    std::vector<unsigned int> cube_indices = {
        0,  1,  2,  2,  3,  0,       // Front
        4,  5,  6,  6,  7,  4,       // Back
        8,  9,  10, 10, 11, 8,       // Left
        12, 13, 14, 14, 15, 12,      // Right
        16, 17, 18, 18, 19, 16,      // Bottom
        20, 21, 22, 22, 23, 20       // Top
    };

    Geometry ret;
    ret.vertices = cube_vertices;
    ret.indices = cube_indices;
    ret.usage = GL_STATIC_DRAW;
    ret.setup();

    return ret;
}

Geometry Sphere(int segments) {
    std::vector<Vertex> sphere_vertices;
    std::vector<unsigned int> sphere_indices;

    int rings = segments;
    int sectors = segments;
    float TWO_PI = 2.0f * PI; // Assuming PI is defined

    for (int r = 0; r <= rings; ++r) {
        float phi = PI * r / rings;
        float y = cosf(phi);
        float sin_phi = sinf(phi);

        for (int s = 0; s <= sectors; ++s) {
            float theta = TWO_PI * s / sectors;
            float x = sin_phi * cosf(theta);
            float z = sin_phi * sinf(theta);

            float u = static_cast<float>(s) / sectors;
            float v = static_cast<float>(r) / rings;

            // Create a Vertex object and push it back
            sphere_vertices.push_back(Vertex{x * 0.5f, y * 0.5f, z * 0.5f, // Position
                                             x, y, z,                   // Normal
                                             u, v});                    // UV
        }
    }

    for (int r = 0; r < rings; ++r) {
        for (int s = 0; s < sectors; ++s) {
            int i0 = r * (sectors + 1) + s;
            int i1 = i0 + 1;
            int i2 = i0 + sectors + 1;
            int i3 = i2 + 1;

            sphere_indices.push_back(i0);
            sphere_indices.push_back(i2);
            sphere_indices.push_back(i1);

            sphere_indices.push_back(i1);
            sphere_indices.push_back(i2);
            sphere_indices.push_back(i3);
        }
    }

    Geometry ret;
    ret.vertices = sphere_vertices;
    ret.indices = sphere_indices;
    ret.usage = GL_STATIC_DRAW;
    ret.setup();

    return ret;
}

void Geometry::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), usage);

    if (!indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);
    }

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(OFFSET_OF(Vertex, position)));
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(OFFSET_OF(Vertex, normal)));
    glEnableVertexAttribArray(1);

    // texture uv
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(OFFSET_OF(Vertex, uv)));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Geometry::Geometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, GLenum usage) {
    this->vertices = vertices;
    this->indices = indices;
    this->usage = usage;
    
    this->setup();
}
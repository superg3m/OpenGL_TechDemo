/*
struct VertexArrayObject {
    unsigned int vao;



    int getStride() const;
    void bind() const;
};

// Date: June 19, 2025
// NOTE(Jovanni): VAO has the attributes to interpret the VBO which has the float data

typedef struct {
    GLuint count;           // Number of indices to draw for this command
    GLuint instanceCount;   // Number of instances to draw for this command
    GLuint firstIndex;      // Index of the first index in the EBO for this command
    GLint  baseVertex;      // Value added to each index to get the final vertex index
    GLuint baseInstance;    // Value for gl_BaseInstance in the shader for this command
} DrawCommand;   
 
// approaching zero driver overhead
glMuliDrawArraysIndirect()
*/
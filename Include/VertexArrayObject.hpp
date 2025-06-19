/*
struct VertexArrayObject {
    unsigned int vao;



    int getStride() const;
    void bind() const;
};

// Date: June 19, 2025
// NOTE(Jovanni): VAO has the attributes to interpret the VBO which has the float data

struct DrawArraysIndirectCommand { 
    GLuint count;
    GLuint primitiveCount;
    GLuint first;
    GLuint baseInstance;
};

struct DrawElementsIndirectCommand { 
    GLuint count;
    GLuint primitiveCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};
 
// approaching zero driver overhead
glMuliDrawArraysIndirect()

std::Vector<VertexAttribute> attributes = {
    VertexAttribute{TD::VEC3, "aPosition"},
    VertexAttribute{TD::VEC3, "aNormal"},
    VertexAttribute{TD::VEC2, "aUV"}
}

std::vector<std::string> shaderUniforms = {
    "uColor",
    "uTexture"
    "uTextureMask"
    "uCubeMap"
}

Shader shader = Shader(paths, attributes, uniforms);
Mesh mesh = Mesh(shader, shaderUnfiforms, attributes)

draw_commands.push(Mesh::Cube(), Mesh::OBJ())

bindable attributes?
bindable shader?
VertexAttributes attributes;
Shader shader;
struct DrawCommand {
    PrimitiveType primitive_type;
    VertexData data;
}


you want push DrawCommands for 
instancing as well.
*/
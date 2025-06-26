#include <shader.hpp>
#include <ckg.h>

unsigned int shader_get_uniform_location(unsigned int program_id, std::string name, const char* path) {
    GLint location = glGetUniformLocation(program_id, name.c_str());
    if (location == -1) {
        CKG_LOG_ERROR("Shader {%s} Uniform: '%s' does not exists\n", path, name.c_str());
    }

    return location;
}

void shader_check_compile_error(unsigned int source_id, const char* path) {
    int success;
    char info_log[1024];
    glGetShaderiv(source_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(source_id, 1024, NULL, info_log);
        CKG_LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR {%s}\n", path);
        CKG_LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
    }
}

GLenum shader_type_from_path(const char* shader_source_path) {
    u64 shader_path_length = ckg_cstr_length(shader_source_path);
    s64 extension_index = ckg_str_last_index_of(shader_source_path, shader_path_length, CKG_LIT_ARG("."));
    ckg_assert_msg(extension_index != -1, "Missing extension (.vert, .frag)\n");
    CKG_StringView extension = ckg_sv_create(shader_source_path + extension_index, shader_path_length - (u64)extension_index);

    if (ckg_str_contains(extension.data, extension.length, CKG_LIT_ARG(".vert"))) {
        return GL_VERTEX_SHADER;
    } else if (ckg_str_contains(extension.data, extension.length, CKG_LIT_ARG(".frag"))) {
        return GL_FRAGMENT_SHADER;
    } else if (ckg_str_contains(extension.data, extension.length, CKG_LIT_ARG(".gs"))) {
        return GL_GEOMETRY_SHADER;
    }

    ckg_assert_msg(false, "Unsupported extension: %.*s | Expected: (.vert, .frag, .gs)\n", extension.length, extension.data);
    return GL_INVALID_ENUM;
}

unsigned int shader_source_compile(const char* path) {
    size_t file_size = 0;
    const GLchar* shader_source = (const GLchar*)ckg_io_read_entire_file(path, &file_size, NULLPTR);
    GLenum type = shader_type_from_path(path);
    unsigned int source_id = glCreateShader(type);
    glShaderSource(source_id, 1, &shader_source, NULL);
    glCompileShader(source_id);

    shader_check_compile_error(source_id, path);

    ckg_free(shader_source);
    return source_id;
}

unsigned int create_shader_program(std::vector<const char*> shader_paths) {
    unsigned int program_id = 0;

    std::vector<unsigned int> shader_sourceIDs; 
    for (const char* path : shader_paths) {
        unsigned int shader_source_id = shader_source_compile(path);
        glAttachShader(program_id, shader_source_id);
        shader_sourceIDs.push_back(shader_source_id);
    }
    glLinkProgram(program_id);

    GLint success = FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[1028] = {0};
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        CKG_LOG_ERROR("LINKING_FAILED\n");
    }

    for (int i = 0; i < shader_sourceIDs.size(); i++) {
        glDeleteShader(shader_sourceIDs[i]);
    }

    return program_id;
}
















/*
void Shader::use() const {
    glUseProgram(this->id);
}

// Date: June 19, 2025
// TODO(Jovanni): Re-enable this in time
int Shader::getUniformLocation(const char* name, const char* path) const {
    GLint location = glGetUniformLocation(this->id, name);
    if (location == -1) {
        CKG_LOG_ERROR("Shader {%s} Uniform: '%s' does not exists\n", path, name);
    }

    return location;
}

void Shader::setBool(const char* name, bool value) {   
    this->use();     

    glUniform1i(this->getUniformLocation(name, this->path), (int)value); 
}

void Shader::setInt(const char* name, int value) { 
    this->use();
    glUniform1i(this->getUniformLocation(name, this->path), value); 
}

void Shader::setFloat(const char* name, float value) { 
    this->use();
    glUniform1f(this->getUniformLocation(name, this->path), value); 
}

void Shader::setVec2(const char* name, const GM_Vec2 &value) {
    this->use();
    glUniform2fv(this->getUniformLocation(name, this->path), 1, &value.x);
}

void Shader::setVec3(const char* name, const GM_Vec3 &value) {
    this->use();
    glUniform3fv(this->getUniformLocation(name, this->path), 1, &value.x);
}

void Shader::setVec3(const char* name, const GM_Vec3* value, int count) {
    this->use();
    glUniform3fv(this->getUniformLocation(name, this->path), count, &value->x);
}

void Shader::setVec3(const char* name, float x, float y, float z) {
    this->use();
    GM_Vec3 temp = GM_Vec3(x, y, z);
    glUniform3fv(this->getUniformLocation(name, this->path), 1, &temp.x);
}

void Shader::setVec4(const char* name, const GM_Vec4 &value) {
    this->use();
    glUniform4fv(this->getUniformLocation(name, this->path), 1, &value.x);
}

void Shader::setVec4(const char* name, const GM_Vec4* value, int count) {
    this->use();
    glUniform4fv(this->getUniformLocation(name, this->path), count, &value->x);
}

void Shader::setIVec4(const char* name, const GM_Vec4 &value) {
    GLint data[4] = {(GLint)value.x, (GLint)value.y, (GLint)value.z, (GLint)value.w};

    this->use();
    glUniform4iv(this->getUniformLocation(name, this->path), 1, data);
}

void Shader::setMat4(const char* name, const GM_Matrix4 &mat) {
    this->use();
    glUniformMatrix4fv(this->getUniformLocation(name, this->path), 1, GL_TRUE, &mat.v[0].x);
}

void Shader::setTexture(std::string name, GLTextureID textureID) {
    this->use();

    GLenum sampler_type = this->uniforms.at(name);
    if (sampler_type != GL_SAMPLER_2D && sampler_type != GL_SAMPLER_CUBE) {
        CKG_LOG_ERROR("Shader {%s} Texture: '%s' is the wrong type in the shader | got: %d\n", this->path, name.c_str(), sampler_type);
        return;
    }

    GLenum texture_type = sampler_type == GL_SAMPLER_2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
    glActiveTexture(GL_TEXTURE0 + this->activeTextureCount);
    glBindTexture(texture_type, textureID);

    GLenum gl_type = this->uniforms.at(name);
    glUniform1i(this->getUniformLocation(name.c_str(), gl_type), this->activeTextureCount); 

    this->activeTextureCount += 1;
}
*/
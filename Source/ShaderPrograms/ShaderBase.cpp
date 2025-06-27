#include <ShaderBase.hpp>
#include <ckg.h>

GLenum ShaderBase::typeFromPath(const char* shader_source_path) {
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

void ShaderBase::checkCompileError(unsigned int source_id, const char* path) {
    int success;
    char info_log[1024];
    glGetShaderiv(source_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(source_id, 1024, NULL, info_log);
        CKG_LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR {%s}\n", path);
        CKG_LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
    }
}

unsigned int ShaderBase::shaderSourceCompile(const char* path) {
    size_t file_size = 0;
    const GLchar* shader_source = (const GLchar*)ckg_io_read_entire_file(path, &file_size, NULLPTR);
    GLenum type = this->typeFromPath(path);
    unsigned int source_id = glCreateShader(type);
    glShaderSource(source_id, 1, &shader_source, NULL);
    glCompileShader(source_id);

    this->checkCompileError(source_id, path);

    ckg_free(shader_source);
    return source_id;
}

unsigned int ShaderBase::getUniformLocation(std::string name) const {
    GLint location = glGetUniformLocation(this->program_id, name.c_str());
    if (location == -1) {
        CKG_LOG_ERROR("Shader Uniform: '%s' does not exists\n", name.c_str());
    }

    return location;
}

unsigned int ShaderBase::getUniformLocation(const char* name) const {
    GLint location = glGetUniformLocation(this->program_id, name);
    if (location == -1) {
        CKG_LOG_ERROR("Shader Uniform: '%s' does not exists\n", name);
    }

    return location;
}

unsigned int ShaderBase::createShaderProgram(std::vector<const char*> shader_paths) {
    unsigned int program_id = glCreateProgram();

    std::vector<unsigned int> shader_sourceIDs; 
    for (const char* path : shader_paths) {
        unsigned int shader_source_id = this->shaderSourceCompile(path);
        glAttachShader(program_id, shader_source_id);
        shader_sourceIDs.push_back(shader_source_id);
    }
    glLinkProgram(program_id);

    GLint success = false;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[1028] = {0};
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        CKG_LOG_ERROR("LINKING_FAILED {%s}\n", shader_paths[0]);
        CKG_LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
    }

    for (int i = 0; i < shader_sourceIDs.size(); i++) {
        glDeleteShader(shader_sourceIDs[i]);
    }

    return program_id;
}

void ShaderBase::use() const {
    glUseProgram(this->program_id);
}

void ShaderBase::setModel(GM_Matrix4 &model) const {
    this->use();
    glUniformMatrix4fv(this->getUniformLocation("uModel"), 1, GL_TRUE, &model.v[0].x);
}

void ShaderBase::setView(GM_Matrix4 &view) const {
    this->use();
    glUniformMatrix4fv(this->getUniformLocation("uView"), 1, GL_TRUE, &view.v[0].x);
}

void ShaderBase::setProjection(GM_Matrix4 &projection) const {
    this->use();
    glUniformMatrix4fv(this->getUniformLocation("uProjection"), 1, GL_TRUE, &projection.v[0].x);
}

void ShaderBase::setMaterial(Material &material) const {
    this->use();

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, material.textures[TEXTURE_TYPE_DIFFUSE]);
    glUniform1i(this->getUniformLocation("uMaterial.diffuse_map"), 0);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, material.textures[TEXTURE_TYPE_SPECULAR]);
    glUniform1i(this->getUniformLocation("uMaterial.specular_map"), 1);

    glUniform1f(this->getUniformLocation("uMaterial.shininess"), material.shininess);
    glUniform1f(this->getUniformLocation("uMaterial.opacity"), material.opacity);
}
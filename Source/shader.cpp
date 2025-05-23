#include <shader.hpp>

#include <ckg.h>
#include <stb_image.h>
#include <glad/glad.h>

typedef enum ShaderType{
    INVALID_SHADER,
    VERTEX_SHADER,
    FRAGMENT_SHADER
} ShaderType;

typedef struct ShaderDescriptor {
    ShaderType type;
    const char* path;
} ShaderDescriptor;

internal void checkCompileError(unsigned int shaderID, const char* type) {
    int success;
    char info_log[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderID, 1024, NULL, info_log);
            CKG_LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: %s\n", type);
            CKG_LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
        }
    } else {
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderID, 1024, NULL, info_log);
            CKG_LOG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: %s\n", type);
            CKG_LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
        }
    }
}

internal ShaderType shaderTypeFromExtension(const char* shader_source_path) {
    u64 shader_path_length = ckg_cstr_length(shader_source_path);
    s64 extension_index = ckg_str_last_index_of(shader_source_path, shader_path_length, CKG_LIT_ARG("."));
    ckg_assert_msg(extension_index != -1, "Missing extension (.vert, .frag)\n");
    CKG_StringView extension = ckg_sv_create(shader_source_path + extension_index, shader_path_length - (u64)extension_index);

    if (ckg_str_contains(extension.data, extension.length, CKG_LIT_ARG(".vert"))) {
        return VERTEX_SHADER;
    } else if (ckg_str_contains(extension.data, extension.length, CKG_LIT_ARG(".frag"))) {
        return FRAGMENT_SHADER;
    }

    ckg_assert_msg(false, "Unsupported extension: %.*s | Expected: (.vert, .frag)\n", extension.length, extension.data);
    return INVALID_SHADER;
}

Shader::Shader(std::vector<const char*> shader_paths) {
    this->id = glCreateProgram();

    std::vector<unsigned int> shaderIDs; 
    for (int i = 0; i < shader_paths.size(); i++) {
        const char* path = shader_paths[i];
        size_t file_size = 0;
        const GLchar* shader_source = (const GLchar*)ckg_io_read_entire_file(path, &file_size, NULLPTR);

        ShaderType type = shaderTypeFromExtension(path);
        unsigned int source_id;
        switch (type) {
            case VERTEX_SHADER: {
                source_id = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(source_id, 1, &shader_source, NULL);
                glCompileShader(source_id);
                checkCompileError(source_id, "VERTEX");
                glAttachShader(this->id, source_id);
            } break;

            case FRAGMENT_SHADER: {
                source_id = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(source_id, 1, &shader_source, NULL);
                glCompileShader(source_id);
                checkCompileError(source_id, "FRAGMENT");
                glAttachShader(this->id, source_id);
            } break;
        }

        ckg_free(shader_source);
        shaderIDs.push_back(source_id);
    }
    glLinkProgram(this->id);

    GLint success = FALSE;
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[1028] = {0};
        glGetProgramInfoLog(this->id, 512, NULL, info_log);
        CKG_LOG_ERROR("LINKING_FAILED\n");
    }

    for (int i = 0; i < shaderIDs.size(); i++) {
        glDeleteShader(shaderIDs[i]);
    }
}

void Shader::addTexture(const char* texture_path, const char* shader_name, int texture_flags) {
    ckg_assert_msg(this->textures.size() <= TEXTURE_MAX, "Texture max hit!");
    ckg_assert_msg(ckg_io_path_exists(texture_path), "Texture path doesn't exist!");

    GLenum MIPMAP_TYPE = GET_BIT(texture_flags, 0) ? GL_NEAREST : GL_LINEAR;
    GLenum TEXTURE_VERTICAL_FLIP = GET_BIT(texture_flags, 1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIPMAP_TYPE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MIPMAP_TYPE);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(TEXTURE_VERTICAL_FLIP);

    u8 *data = stbi_load(texture_path, &width, &height, &nrChannels, 0);

    GLenum format = 0;
    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    } else {
        CKG_LOG_ERROR("Failed to pick a stb format\n");
        ckg_assert(FALSE);
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        CKG_LOG_ERROR("Failed to load texture\n");
    }

    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(FALSE);

    this->textures.push_back(texture);
    this->setInt(shader_name, this->textures.size() - 1);
}

void Shader::bindTextures() {
    this->use();
    for (int i = 0; i < this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i]);
    }
}

void Shader::unbindTextures() {
    this->use();
    for (int i = 0; i < this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Shader::use() {
    glUseProgram(this->id);
}

void Shader::setBool(const char* name, bool value) {   
    this->use();      
    glUniform1i(glGetUniformLocation(this->id, name), (int)value); 
}

void Shader::setInt(const char* name, int value) { 
    this->use();
    glUniform1i(glGetUniformLocation(this->id, name), value); 
}

void Shader::setFloat(const char* name, float value) { 
    this->use();
    glUniform1f(glGetUniformLocation(this->id, name), value); 
}

void Shader::setMat4(const char* name, const GM_Matrix4 &mat) {
    this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_TRUE, &mat.data[0]);
}

void Shader::setVec2(const char* name, const GM_Vec2 &value) {
    this->use();
    glUniform2fv(glGetUniformLocation(this->id, name), 1, &value.v[0]);
}

void Shader::setVec3(const char* name, const GM_Vec3 &value) {
    this->use();
    glUniform3fv(glGetUniformLocation(this->id, name), 1, &value.v[0]);
}

void Shader::setVec4(const char* name, const GM_Vec4 &value) {
    this->use();
    glUniform4fv(glGetUniformLocation(this->id, name), 1, &value.v[0]);
}
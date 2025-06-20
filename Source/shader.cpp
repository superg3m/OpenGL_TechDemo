#include <ckg.h>
#include <shader.hpp>
#include <stb_image.h>

typedef enum ShaderType{
    INVALID_SHADER,
    VERTEX_SHADER,
    FRAGMENT_SHADER
} ShaderType;

typedef struct ShaderDescriptor {
    ShaderType type;
    const char* path;
} ShaderDescriptor;

void Shader::checkCompileError(unsigned int shaderID, const char* type) {
    int success;
    char info_log[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderID, 1024, NULL, info_log);
            CKG_LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR {%s} of type: %s\n", this->path, type);
            CKG_LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
        }
    } else {
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderID, 1024, NULL, info_log);
            CKG_LOG_ERROR("ERROR::PROGRAM_LINKING_ERROR {%s}  of type: %s\n", this->path, type);
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

Shader::Shader(std::vector<const char*> shader_paths, std::vector<std::string> uniforms, std::map<std::string, TextureType> textures) {
    this->id = glCreateProgram();
    this->uniforms = uniforms;
    this->textures = textures;
    this->path = shader_paths[0];

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

void Shader::use() const {
    glUseProgram(this->id);
}

// Date: June 19, 2025
// TODO(Jovanni): Re-enable this in time
GLint Shader::getUniformLocation(const char* name) const {
    GLint location = glGetUniformLocation(this->id, name);
    if (location == -1) {
        CKG_LOG_ERROR("Shader {%s} Uniform: '%s' does not exists\n", this->path, name);
    }

    return location;
}

void Shader::setBool(const char* name, bool value) {   
    this->use();      
    glUniform1i(this->getUniformLocation(name), (int)value); 
}

void Shader::setInt(const char* name, int value) { 
    this->use();
    glUniform1i(this->getUniformLocation(name), value); 
}

void Shader::setFloat(const char* name, float value) { 
    this->use();
    glUniform1f(this->getUniformLocation(name), value); 
}

void Shader::setVec2(const char* name, const GM_Vec2 &value) {
    this->use();
    glUniform2fv(this->getUniformLocation(name), 1, &value.x);
}

void Shader::setVec3(const char* name, const GM_Vec3 &value) {
    this->use();
    glUniform3fv(this->getUniformLocation(name), 1, &value.x);
}

void Shader::setVec3(const char* name, float x, float y, float z) {
    this->use();
    GM_Vec3 temp = GM_Vec3(x, y, z);
    glUniform3fv(this->getUniformLocation(name), 1, &temp.x);
}

void Shader::setVec4(const char* name, const GM_Vec4 &value) {
    this->use();
    glUniform4fv(this->getUniformLocation(name), 1, &value.x);
}

void Shader::setIVec4(const char* name, const GM_Vec4 &value) {
    GLint data[4] = {(GLint)value.x, (GLint)value.y, (GLint)value.z, (GLint)value.w};

    this->use();
    glUniform4iv(this->getUniformLocation(name), 1, data);
}

void Shader::setMat4(const char* name, const GM_Matrix4 &mat) {
    this->use();
    glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_TRUE, &mat.v[0].x);
}

void Shader::setMat4(const char* name, const glm::mat4 &mat) const {
    this->use();
    glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::bindTexture(std::string name, GLTextureID textureID) {
    this->use();
    TextureType type = this->textures[name];
    glActiveTexture(GL_TEXTURE0 + this->activeTextureCount);
    if (type == TextureType::SAMPLER2D) {
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else if (type == TextureType::CUBEMAP ) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    }
    this->setInt(name.c_str(), this->activeTextureCount);
    
    this->activeTextureCount += 1;
}

void Shader::unbindTextures() {
    #define TEXTURE_MAX 32

    this->use();
    for (int i = 0; i < TEXTURE_MAX; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    this->activeTextureCount = 0;
}
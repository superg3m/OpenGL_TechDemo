#include <vector>
#include <glad/glad.h>
#include <gm.hpp>
#include <material.hpp>
#include <ckg.h>

enum ShaderType{
    INVALID_SHADER,
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER
};

struct ShaderDescriptor {
    ShaderType type;
    const char* path;
};

struct ShaderModel {
    ShaderModel();

    unsigned int program_id;

    void use();
    void setModel(GM_Matrix4 &model);
    void setView(GM_Matrix4 &view);
    void setProjection(GM_Matrix4 &projection);

    void setMaterial(Material &material);
    void setLightPosition(GM_Vec3 &position, int index);
    void setLightColor(GM_Vec3 &color, int index);
    void setViewPosition(GM_Vec3 &view_position);
private:
    GM_Matrix4 uModel;
    GM_Matrix4 uView;
    GM_Matrix4 uProjection;

    Material uMaterial;
    GM_Vec3 uLightPositions[4];
    GM_Vec3 uLightColors[4];

    GM_Vec3 uViewPosition;

    // Vertex Uniforms
    unsigned int uModel_Location;
    unsigned int uView_Location;
    unsigned int uProjection_Location;

    // Fragment Uniforms
    unsigned int uMaterial_Location;
    unsigned int uLightPositions_Location;
    unsigned int uLightColors_Location;
    unsigned int uViewPosition_Location;
    unsigned int uGamma_Location;

    int getUniformLocation(const char* name, const char* path) const;
};

internal GLenum shaderTypeFromExtension(const char* shader_source_path) {
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

internal unsigned int shaderSourceCompile(const char* path) {
    size_t file_size = 0;
    const GLchar* shader_source = (const GLchar*)ckg_io_read_entire_file(path, &file_size, NULLPTR);
    GLenum type = shaderTypeFromExtension(path);
    unsigned int source_id = glCreateShader(type);
    glShaderSource(source_id, 1, &shader_source, NULL);
    glCompileShader(source_id);

    int success;
    char info_log[1024];
    glGetShaderiv(source_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(source_id, 1024, NULL, info_log);
        CKG_LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR {%s}\n", path);
        CKG_LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
    }

    ckg_free(shader_source);
    return source_id;
}

int ShaderModel::getUniformLocation(const char* name, const char* path) const {
    GLint location = glGetUniformLocation(this->program_id, name);
    if (location == -1) {
        CKG_LOG_ERROR("Shader {%s} Uniform: '%s' does not exists\n", path, name);
    }

    return location;
}

ShaderModel::ShaderModel() {
    this->program_id = glCreateProgram();
    std::vector<const char*> shader_paths = {"../../shader_source/model/model.vert", "../../shader_source/model/model.frag"};
    std::vector<unsigned int> shader_sourceIDs; 
    for (const char* path : shader_paths) {
        unsigned int shader_source_id = shaderSourceCompile(path);
        glAttachShader(this->program_id, shader_source_id);
        shader_sourceIDs.push_back(shader_source_id);
    }
    glLinkProgram(this->program_id);

    GLint success = FALSE;
    glGetProgramiv(this->program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[1028] = {0};
        glGetProgramInfoLog(this->program_id, 512, NULL, info_log);
        CKG_LOG_ERROR("LINKING_FAILED\n");
    }

    for (int i = 0; i < shader_sourceIDs.size(); i++) {
        glDeleteShader(shader_sourceIDs[i]);
    }

    this->uModel_Location = this->getUniformLocation("uModel", shader_paths[i]);
    this->uView_Location = this->getUniformLocation("uView", shader_paths[i]);
    this->uProjection_Location = this->getUniformLocation("uProjection", shader_paths[i]);

    this->uMaterial_Location = this->getUniformLocation("uMaterial", shader_paths[i]);

    this->uLightPositions_Location = this->getUniformLocation("lightPositions[0]", shader_paths[i]);
    this->uLightColors_Location = this->getUniformLocation("lightColors[0]", shader_paths[i]);
    this->uViewPosition_Location = this->getUniformLocation("viewPosition", shader_paths[i]);
    this->uGamma_Location = this->getUniformLocation("gamma", shader_paths[i]);
}
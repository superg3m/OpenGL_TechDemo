#include <shader.h>
#include <stb_image.h>

internal void shader_check_compile_errors(u32 shaderID, const char* type) {
    int success;
    char info_log[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderID, 1024, NULL, info_log);
            LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: %s\n", type);
            LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
        }
    } else {
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderID, 1024, NULL, info_log);
            LOG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: %s\n", type);
            LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
        }
    }
}

internal ShaderType shader_type_from_extension(const char* shader_source_path) {
    u64 shader_source_path_length = ckg_cstr_length(shader_source_path);
    s64 extension_index = ckg_cstr_last_index_of(shader_source_path, shader_source_path_length, CKG_LIT_ARG("."));
    ckit_assert_msg(extension_index != -1, "Missing extension (.vert, .frag)\n");

    String extension = ckit_str_create_custom(shader_source_path + extension_index, shader_source_path_length - extension_index, 0);
    if (ckg_cstr_contains((char*)extension, ckit_str_length(extension), CKG_LIT_ARG(".vert"))) {
        return VERTEX_SHADER;
    } else if (ckg_cstr_contains((char*)extension, ckit_str_length(extension), CKG_LIT_ARG(".frag"))) {
        return FRAGMENT_SHADER;
    }

    ckit_assert_msg(FALSE, "Unsupported extension: %s | Expected: (.vert, .frag)\n", extension);
    return INVALID_SHADER;
}

// Date: August 15, 2024
// NOTE(Jovanni): You might not even need this shader_discriptors
Shader shader_create(const char** shader_source_paths, u32 shader_source_path_count) {
    Shader ret = {0};
    ret.textures = ckit_vector_reserve(1, u32);
    ret.attributes = ckit_vector_reserve(1, u32);
    u32* shader_source_ids = NULLPTR; 

    ret.id = glCreateProgram();
    for (u32 i = 0; i < shader_source_path_count; i++) {
        const char* path = shader_source_paths[i];
        size_t file_size = 0;
        u8* shader_source = (u8*)ckit_os_read_entire_file(path, &file_size);
        const GLchar* const_shader_source = (const GLchar*)shader_source;
        ShaderType type = shader_type_from_extension(path);
        u32 source_id;

        switch (type) {
            case VERTEX_SHADER: {
                // Determine the vertex_attribute_components
                // ret.attributes = 

                source_id = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(source_id, 1, &const_shader_source, NULL);
                glCompileShader(source_id);
                shader_check_compile_errors(source_id, "VERTEX");
                glAttachShader(ret.id, source_id);
            } break;

            case FRAGMENT_SHADER: {
                source_id = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(source_id, 1, &const_shader_source, NULL);
                glCompileShader(source_id);
                shader_check_compile_errors(source_id, "FRAGMENT");
                glAttachShader(ret.id, source_id);
            } break;
        }

        ckit_free(shader_source);
        ckit_vector_push(shader_source_ids, source_id);
    }
    glLinkProgram(ret.id);

    GLint success = FALSE;
    glGetProgramiv(ret.id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[1028] = {0};
        glGetProgramInfoLog(ret.id, 512, NULL, info_log);
        LOG_ERROR("LINKING_FAILED\n");
    }

    for (u32 i = 0; i < ckit_vector_count(shader_source_ids); i++) {
        glDeleteShader(shader_source_ids[i]);
    }
    ckit_vector_free(shader_source_ids);

    return ret;
}

void shader_free(Shader* shader) {
    if (shader->textures) {
        ckit_vector_free(shader->textures);
    }

    ckit_vector_free(shader->attributes);
}

void shader_add_texture(Shader* shader, const char* texture_path, const char* shader_name, u8 texture_flags) {
    if (shader->textures) {
        ckit_assert_msg(ckit_vector_count(shader->textures) <= TEXTURE_MAX, "Texture max hit!");
    }
    ckit_assert_msg(ckit_os_path_exists(texture_path), "Texture path doesn't exist!");

    GLenum MIPMAP_TYPE = GET_BIT(texture_flags, 0) ? GL_NEAREST : GL_LINEAR;
    GLenum TEXTURE_VERTICAL_FLIP = GET_BIT(texture_flags, 1);

    u32 texture;
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
        ckit_assert(FALSE);
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        LOG_ERROR("Failed to load texture\n");
    }

    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(FALSE);

    ckit_vector_push(shader->textures, texture);
    shader_set_int(shader, shader_name, ckit_vector_count(shader->textures) - 1);
}

void shader_bind_textures(Shader* shader) {
    shader_use(shader);
    for (u32 i = 0; i < ckit_vector_count(shader->textures); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, shader->textures[i]);
    }
}

void shader_unbind_textures(Shader* shader) {
    shader_use(shader);
    for (u32 i = 0; i < ckit_vector_count(shader->textures); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void shader_use(Shader* shader) {
    glUseProgram(shader->id);
}

void shader_set_bool(Shader* shader, const char* name, Boolean value) {   
    shader_use(shader);      
    glUniform1i(glGetUniformLocation(shader->id, name), (int)value); 
}

void shader_set_int(Shader* shader, const char* name, int value) { 
    shader_use(shader);
    glUniform1i(glGetUniformLocation(shader->id, name), value); 
}

void shader_set_float(Shader* shader, const char* name, float value) { 
    shader_use(shader);
    glUniform1f(glGetUniformLocation(shader->id, name), value); 
}

void shader_set_mat2(Shader* shader, const char* name, const glm::mat2 &mat) {
    shader_use(shader);
    glUniformMatrix2fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, &mat[0][0]);
}

void shader_set_mat3(Shader* shader, const char* name, const glm::mat3 &mat) {
    shader_use(shader);
    glUniformMatrix3fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, &mat[0][0]);
}

void shader_set_mat4(Shader* shader, const char* name, const glm::mat4 &mat) {
    shader_use(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, &mat[0][0]);
}

void shader_set_vec2(Shader* shader, const char* name, const glm::vec2 &value) {
    shader_use(shader);
    glUniform2fv(glGetUniformLocation(shader->id, name), 1, &value[0]);
}

void shader_set_vec3(Shader* shader, const char* name, const glm::vec3 &value) {
    shader_use(shader);
    glUniform3fv(glGetUniformLocation(shader->id, name), 1, &value[0]);
}

void shader_set_vec4(Shader* shader, const char* name, const glm::vec4 &value) {
    shader_use(shader);
    glUniform4fv(glGetUniformLocation(shader->id, name), 1, &value[0]);
}
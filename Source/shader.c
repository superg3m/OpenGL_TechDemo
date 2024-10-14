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
    ckit_assert_msg(ckit_str_contains(shader_source_path, "."), "Missing extension (.vert, .frag)");

    u32 str_length = ckit_cstr_length(shader_source_path);
    String extension = ckit_substring(shader_source_path, ckit_str_last_index_of(shader_source_path, "."), str_length);
    if (ckit_str_contains(extension, ".vert")) {
        return VERTEX_SHADER;
    } else if (ckit_str_contains(extension, ".frag")) {
        return FRAGMENT_SHADER;
    }

    ckit_assert_msg(FALSE, "Unsupported extension: %s | Expetcted: (.vert, .frag)", extension);
    return INVALID_SHADER;
}

// Date: August 15, 2024
// NOTE(Jovanni): You might not even need this shader_discriptors
Shader shader_create(const char** shader_source_path, u32 shader_source_path_count) {
    Shader ret = {0};
    u32* shader_source_ids = NULLPTR; 

    ret.id = glCreateProgram();
    for (int i = 0; i < shader_source_path_count; i++) {
        const char* path = shader_source_path[i];
        size_t file_size = 0;
        const char* shader_source = ckit_os_read_entire_file(path, &file_size);
        ShaderType type = shader_type_from_extension(path);
        u32 source_id;

        switch (type) {
            case VERTEX_SHADER: {
                source_id = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(source_id, 1, &shader_source, NULL);
                glCompileShader(source_id);
                shader_check_compile_errors(source_id, "VERTEX");
                glAttachShader(ret.id, source_id);
            } break;

            case FRAGMENT_SHADER: {
                source_id = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(source_id, 1, &shader_source, NULL);
                glCompileShader(source_id);
                shader_check_compile_errors(source_id, "FRAGMENT");
                glAttachShader(ret.id, source_id);
            } break;
        }

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

    for (int i = 0; i < ckit_vector_count(shader_source_ids); i++) {
        glDeleteShader(shader_source_ids[i]);
    }

    return ret;
}

void shader_add_texture(Shader* shader, const char* texture_path) {
    ckit_assert_msg(ckit_vector_count(shader->textures) <= TEXTURE_MAX, "Texture max hit!");

    u32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    u8 *data = stbi_load(texture_path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        LOG_ERROR("Failed to load texture\n");
    }
    stbi_image_free(data);

    ckit_vector_push(shader->textures, texture);
}

void shader_bind_textures(Shader* shader) {
    for (u32 i = 0; i < ckit_vector_count(shader->textures); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, shader->textures[i]);
    }
}

void shader_use(Shader* shader) {
    glUseProgram(shader->id);
}
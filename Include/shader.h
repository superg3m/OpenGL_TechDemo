#pragma once

#include <ckit.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TEXTURE_MAX 32

typedef enum TextureFlag {
    TEXTURE_DEFAULT = 0,
    TEXTURE_PIXEL_PERFECT = 1,
    TEXTURE_VERTICAL_FLIP = 2
} TextureFlag;

typedef enum ShaderType{
    INVALID_SHADER,
    VERTEX_SHADER,
    FRAGMENT_SHADER
} ShaderType;

typedef struct ShaderDescriptor {
    ShaderType type;
    const char* path;
} ShaderDescriptor;

typedef struct Shader {
    u32* textures;
    u32* attributes;
    u32 id;
} Shader;

Shader shader_create(const char** shader_source_path, u32 shader_source_path_count);
void shader_free(Shader* shader);
void shader_add_texture(Shader* shader, const char* texture_path, const char* shader_name, u8 texture_flags);
void shader_bind_textures(Shader* shader);
void shader_unbind_textures(Shader* shader);
void shader_use(Shader* shader);
void shader_set_bool(Shader* shader, const char* name, Boolean value);
void shader_set_int(Shader* shader, const char* name, int value);
void shader_set_float(Shader* shader, const char* name, float value);
void shader_set_mat2(Shader* shader, const char* name, const glm::mat2 &mat);
void shader_set_mat3(Shader* shader, const char* name, const glm::mat3 &mat);
void shader_set_mat4(Shader* shader, const char* name, const glm::mat4 &mat);
void shader_set_vec2(Shader* shader, const char* name, const glm::vec2 &value);
void shader_set_vec3(Shader* shader, const char* name, const glm::vec3 &value);
void shader_set_vec4(Shader* shader, const char* name, const glm::vec4 &value);
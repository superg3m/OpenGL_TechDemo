#pragma once

#include <ckit.h>
#include <glad/glad.h>

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
#include <ckit.h>
#include <glad/glad.h>

#define TEXTURE_MAX 32

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
void shader_add_texture(Shader* shader, const char* texture_path);
void shader_bind_textures(Shader* shader);
void shader_use(Shader* shader);
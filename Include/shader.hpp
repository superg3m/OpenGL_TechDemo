#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TEXTURE_MAX 32

typedef enum TextureFlag {
    TEXTURE_DEFAULT = 0x0,
    TEXTURE_PIXEL_PERFECT = 0x1,
    TEXTURE_VERTICAL_FLIP = 0x2
} TextureFlag;

struct Shader {
    std::vector<unsigned int> textures;
    std::vector<unsigned int> attributes;
    unsigned int id;

    Shader(std::vector<const char*> shader_paths);

    void addTexture(const char* texture_path, const char* shader_name, int texture_flags);
    void bindTextures();
    void unbindTextures();

    void use();
    void setBool(const char* name, bool value);
    void setInt(const char* name, int value);
    void setFloat(const char*name, float value);
    void setMat2(const char*name, const glm::mat2 &mat);
    void setMat3(const char*name, const glm::mat3 &mat);
    void setMat4(const char*name, const glm::mat4 &mat);
    void setVec2(const char*name, const glm::vec2 &value);
    void setVec3(const char*name, const glm::vec3 &value);
    void setVec4(const char*name, const glm::vec4 &value);
};
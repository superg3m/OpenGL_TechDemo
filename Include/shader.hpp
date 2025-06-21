#pragma once

#include <string>
#include <vector>
#include <map>

#include <gm.hpp>

#include <glad/glad.h>

enum class TextureType {
    SAMPLER2D,
    CUBEMAP
}; 

using GLTextureID = int;
struct Shader {
    unsigned int id;
    const char* path;
    std::map<std::string, TextureType> textures;
    int activeTextureCount;
    Shader() = default;
    Shader(std::vector<const char*> shader_paths, std::map<std::string, TextureType> textures);

    void use() const;
    void setBool(const char* name, bool value);
    void setInt(const char* name, int value);
    void setFloat(const char*name, float value);
    void setVec2(const char*name, const GM_Vec2 &value);
    void setVec3(const char*name, const GM_Vec3 &value);
    void setVec3(const char* name, float x, float y, float z);
    void setVec4(const char*name, const GM_Vec4 &value);
    void setIVec4(const char*name, const GM_Vec4 &value);
    void setMat4(const char*name, const GM_Matrix4 &mat);

    void bindTexture(std::string name, GLTextureID textureID);
    void unbindTextures();

    static Shader BasicShader();
    static Shader StandardShader();
private:
    std::map<std::string, GLenum> uniforms;
    void setTexture(const char* name, int value);
    GLint getAttributeLocation(const char* name) const;
    GLint getUniformLocation(const char* name, GLenum type) const;
    void checkCompileError(unsigned int shaderID, const char* type);
};
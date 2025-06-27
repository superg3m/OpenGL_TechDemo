#pragma once

#include <vector>
#include <string>
#include <Material.hpp>
#include <gm.hpp>

#include <glad/glad.h>

struct ShaderBase {
    unsigned int program_id = 0;
    virtual void compile() = 0;

    void use() const;

    // Vertex Uniforms
    void setModel(GM_Matrix4 &model) const;
    void setView(GM_Matrix4 &view) const;
    void setProjection(GM_Matrix4 &projection) const;

    // Fragment Uniforms
    void setMaterial(Material &material) const;
protected:
    std::vector<const char*> shader_paths;
    ShaderBase() = default;
    GLenum typeFromPath(const char* path);
    void checkCompileError(unsigned int source_id, const char* path);
    unsigned int shaderSourceCompile(const char* path);
    unsigned int getUniformLocation(const char* name) const;
    unsigned int getUniformLocation(std::string name) const;
    unsigned int createShaderProgram(std::vector<const char*> shader_paths);
};
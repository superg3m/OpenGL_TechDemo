#pragma once

#include <ShaderBase.hpp>

struct ShaderUniformColor : public ShaderBase {
    ShaderUniformColor() = default;
    ShaderUniformColor(std::vector<const char*> shader_paths);
    void compile() override;
    void setColor(GM_Vec3 color);
private:
    unsigned int uColor_Location = 0;
};
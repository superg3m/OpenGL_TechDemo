#pragma once

#include <ShaderBase.hpp>

struct ShaderUniformColor : public ShaderBase {
    ShaderUniformColor() = default;
    void init() override;

    void setColor(GM_Vec3 color);
private:
    unsigned int uColor_Location = 0;
};
#pragma once

#include <ShaderBase.hpp>

struct ShaderUniformColor : public ShaderBase {
    ShaderUniformColor();
    void setColor(GM_Vec3 &color);
private:
    unsigned int uColor_Location = 0;
};
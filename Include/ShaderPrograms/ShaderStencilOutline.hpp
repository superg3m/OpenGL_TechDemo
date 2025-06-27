#pragma once

#include <ShaderBase.hpp>

struct ShaderStencilOutline : public ShaderBase {
    ShaderStencilOutline() = default;;
    void init() override;
    
    void setOutlineScale(float s) const;

private:
    unsigned int uOutlineScale_Location = 0;
};
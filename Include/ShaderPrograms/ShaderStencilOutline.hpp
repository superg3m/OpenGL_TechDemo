#pragma once

#include <ShaderBase.hpp>

struct ShaderStencilOutline : public ShaderBase {
    ShaderStencilOutline() = default;
    ShaderStencilOutline(std::vector<const char*> shader_paths);
    void compile() override;
    
    void setOutlineScale(float s) const;

private:
    unsigned int uOutlineScale_Location = 0;
};
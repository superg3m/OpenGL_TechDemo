#pragma once

#include <vector>
#include <ShaderBase.hpp>

struct ShaderTransparency : public ShaderBase {
    ShaderTransparency() = default;
    ShaderTransparency(std::vector<const char*> shader_paths);
    void compile() override;
    
    void setTexture(GLTextureID texture) const;
    void setOpacity(float opacity) const;
};
#pragma once

#include <vector>
#include <ShaderBase.hpp>

struct ShaderTransparency : public ShaderBase {
    ShaderTransparency() = default;;
    void init() override;
    
    void setTexture(GLTextureID texture) const;
    void setOpacity(float opacity) const;
};
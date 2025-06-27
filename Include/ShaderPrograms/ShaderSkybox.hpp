#pragma once

#include <vector>
#include <ShaderBase.hpp>

struct ShaderSkybox : public ShaderBase {
    ShaderSkybox() = default;;
    void init() override;
    
    void setSkyboxTexture(GLTextureID texture) const;
};
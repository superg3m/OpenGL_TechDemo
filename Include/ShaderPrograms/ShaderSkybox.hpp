#pragma once

#include <vector>
#include <ShaderBase.hpp>

struct ShaderSkybox : public ShaderBase {
    ShaderSkybox() = default;
    ShaderSkybox(std::vector<const char*> shader_paths);
    void compile() override;
    
    void setSkyboxTexture(GLTextureID texture) const;
};
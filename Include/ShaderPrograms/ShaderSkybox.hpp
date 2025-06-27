#pragma once

#include <vector>
#include <ShaderBase.hpp>

struct ShaderSkybox : public ShaderBase {
    ShaderSkybox();

    void setSkyboxTexture(GLTextureID texture) const;
};
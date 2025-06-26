#include <vector>
#include <glad/glad.h>
#include <gm.hpp>
#include <material.hpp>
#include <ckg.h>

struct ShaderSkybox {
    ShaderSkybox();

    unsigned int program_id;

    void use();

    // Vertex Uniforms
    void setView(GM_Matrix4 &view);
    void setProjection(GM_Matrix4 &projection);

    // Fragment Uniforms
    void setSkyboxTexture(GLTextureID texture);
private:
    // Vertex Uniforms
    unsigned int uView_Location;
    unsigned int uProjection_Location;

    // Fragment Uniforms
    unsigned int uSkyboxTexture_Location;

    int getUniformLocation(const char* name, const char* path) const;
};
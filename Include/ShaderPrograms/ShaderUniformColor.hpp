#include <vector>
#include <glad/glad.h>
#include <gm.hpp>
#include <material.hpp>
#include <ckg.h>

struct ShaderUniformColor {
    ShaderUniformColor();

    unsigned int program_id;

    void use();
    void setModel(GM_Matrix4 &model);
    void setView(GM_Matrix4 &view);
    void setProjection(GM_Matrix4 &projection);

    void setColor(GM_Vec3 &color);
private:
    // Vertex Uniforms
    unsigned int uModel_Location;
    unsigned int uView_Location;
    unsigned int uProjection_Location;

    // Fragment Uniforms
    unsigned int uColor_Location;

    int getUniformLocation(const char* name, const char* path) const;
};
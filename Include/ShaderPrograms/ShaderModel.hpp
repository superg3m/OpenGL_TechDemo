#include <vector>
#include <gm.hpp>
#include <material.hpp>

#define LIGHT_COUNT 4

struct ShaderModel {
    ShaderModel();

    unsigned int program_id;

    void use() const;

    // Vertex Uniforms
    void setModel(GM_Matrix4 &model) const;
    void setView(GM_Matrix4 &view) const;
    void setProjection(GM_Matrix4 &projection) const;

    // Fragment Uniforms
    void setMaterial(Material &material) const;
    void setLightPosition(GM_Vec3 &position, int index) const;
    void setLightColor(GM_Vec3 &color, int index) const;
    void setViewPosition(GM_Vec3 &view_position) const;

    void setGamma(bool &gamma) const;
private:
    // Vertex Uniforms
    unsigned int uModel_Location;
    unsigned int uView_Location;
    unsigned int uProjection_Location;

    // Fragment Uniforms
    unsigned int uMaterial_Location;
    unsigned int uLightPosition_Locations[LIGHT_COUNT];
    unsigned int uLightColor_Locations[LIGHT_COUNT];
    unsigned int uViewPosition_Location;
    unsigned int uGamma_Location;

    int getUniformLocation(const char* name, const char* path) const;
};
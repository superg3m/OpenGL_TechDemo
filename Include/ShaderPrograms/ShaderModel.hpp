#include <ShaderBase.hpp>

#define LIGHT_COUNT 4

struct PointLightLocation {
    unsigned int position;

    unsigned int constant;
    unsigned int linear;
    unsigned int quadratic;

    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};

struct PointLight {
    GM_Vec3 position;

    float constant;
    float linear;
    float quadratic;

    GM_Vec3 ambient;
    GM_Vec3 diffuse;
    GM_Vec3 specular;
};

struct ShaderModel : public ShaderBase {
    ShaderModel() = default;
    void init() override;
    
    // Fragment Uniforms
    void setPointLight(PointLight &point_light, int index) const;
    void setViewPosition(GM_Vec3 &view_position) const;
private:
    // Fragment Uniforms
    PointLightLocation uPointLight_Locations[LIGHT_COUNT];
    unsigned int uViewPosition_Location;
};
#include <ShaderBase.hpp>

#define LIGHT_COUNT 4

struct SpotLightLocation {
    unsigned int position;
    unsigned int direction;
    unsigned int cutOff;
    unsigned int outerCutOff;

    unsigned int constant;
    unsigned int linear;
    unsigned int quadratic;
 
    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};

struct SpotLight {
    GM_Vec3 position;
    GM_Vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
 
    GM_Vec3 ambient;
    GM_Vec3 diffuse;
    GM_Vec3 specular;        
};

struct DirectionalLightLocation {
    unsigned int direction;

    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};

struct DirectionalLight {
    GM_Vec3 direction;

    GM_Vec3 ambient;
    GM_Vec3 diffuse;
    GM_Vec3 specular;
};

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
    ShaderModel(std::vector<const char*> shader_paths);
    void compile() override;
    
    // Fragment Uniforms
    void setSpotLight(SpotLight &spot_light) const;
    void setDirectionalLight(DirectionalLight &directional_light) const;
    void setPointLight(PointLight &point_light, int index) const;
    void setViewPosition(GM_Vec3 &view_position) const;
    void setUseFlashlight(bool useFlashlight) const;
private:
    // Fragment Uniforms
    SpotLightLocation uSpotLight_Location;
    DirectionalLightLocation uDirectionalLight_Location;
    PointLightLocation uPointLight_Locations[LIGHT_COUNT];
    unsigned int uViewPosition_Location;
    unsigned int uUseFlashlight_Location;
};
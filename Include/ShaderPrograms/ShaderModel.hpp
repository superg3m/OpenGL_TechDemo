#include <ShaderBase.hpp>

#define LIGHT_COUNT 4

struct ShaderModel : public ShaderBase {
    ShaderModel() = default;
    void init() override;
    
    // Fragment Uniforms
    void setLightPosition(GM_Vec3 &position, int index) const;
    void setLightColor(GM_Vec3 &color, int index) const;
    void setViewPosition(GM_Vec3 &view_position) const;

    void setGamma(bool &gamma) const;
private:
    // Fragment Uniforms
    unsigned int uLightPosition_Locations[LIGHT_COUNT];
    unsigned int uLightColor_Locations[LIGHT_COUNT];
    unsigned int uViewPosition_Location;
    unsigned int uGamma_Location;
};
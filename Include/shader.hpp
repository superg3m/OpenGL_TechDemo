#pragma once

#include <vector>
#include <gm.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

struct Shader {
    unsigned int id;
    Shader(std::vector<const char*> shader_paths);

    void use();
    void setBool(const char* name, bool value);
    void setInt(const char* name, int value);
    void setFloat(const char*name, float value);
    void setMat4(const char*name, const GM_Matrix4 &mat);
    void setVec2(const char*name, const GM_Vec2 &value);
    void setVec3(const char*name, const GM_Vec3 &value);
    void setVec3(const char* name, float x, float y, float z);
    void setVec4(const char*name, const GM_Vec4 &value);
};
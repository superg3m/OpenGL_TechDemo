#pragma once

#include <gm.hpp>

struct MousePicker {
    GM_Vec3 rayOrigin;
    GM_Vec3 rayDirection;
    MousePicker() = default;
    void update(GM_Matrix4 projection, GM_Matrix4 view);
    GM_Vec3 getFromObjectZ(GM_Matrix4 projection, GM_Matrix4 view, float object_z);
private:
    GM_Matrix4 projection;
    GM_Matrix4 view;
};
#pragma once

#include <gm.hpp>

struct MousePicker {
    GM_Vec3 ray;
    MousePicker() = default;
    void update(GM_Matrix4 projection, GM_Matrix4 view);
private:
    GM_Matrix4 projection;
    GM_Matrix4 view;

    GM_Vec2 toNormalizedDeviceSpace(float screen_space_mouse_x, float screen_space_mouse_y);
    GM_Vec4 toClipSpace(GM_Vec2 ndc);
    GM_Vec4 toViewSpace(GM_Vec4 clipsSpace);
    GM_Vec3 toWorldSpace(GM_Vec4 viewSpace);
};
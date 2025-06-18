#include <mouse_picker.hpp>
#include <IOD.hpp>
#include <game.hpp>

void MousePicker::update(GM_Matrix4 projection, GM_Matrix4 view) {
    this->projection = projection;
    this->view = view;

    // Date: June 18, 2025
    // NOTE(Jovanni): NDC are from -1 to 1
    // also ndc y might have to be negated: -ndc_y

    float mouse_x = IOD::getMouseX();
    float mouse_y = IOD::getMouseY();

    GM_Vec2 ndc = this->toNormalizedDeviceSpace(mouse_x, mouse_y);
    GM_Vec4 clipSpace = this->toClipSpace(ndc);
    GM_Vec4 viewSpace = this->toViewSpace(clipSpace);

    this->ray = this->toWorldSpace(viewSpace).normalize();
}

GM_Vec2 MousePicker::toNormalizedDeviceSpace(float screen_space_mouse_x, float screen_space_mouse_y) {
    float ndc_x = ((2.0f * screen_space_mouse_x) / Game::WINDOW_WIDTH) - 1.0f;
    float ndc_y = ((2.0f * screen_space_mouse_y) / Game::WINDOW_HEIGHT) - 1.0f;

    return GM_Vec2(ndc_x, ndc_y);
}
GM_Vec4 MousePicker::toClipSpace(GM_Vec2 ndc) {
    return GM_Vec4(ndc.x, ndc.y, -1, 1);
}

GM_Vec4 MousePicker::toViewSpace(GM_Vec4 clipSpace) {
    bool success = false;
    GM_Matrix4 inverseProjection = GM_Matrix4::inverse(this->projection, &success);
    if (!success) {
        CKG_LOG_ERROR("Failed to invert projection matrix\n");
        return GM_Vec4(0, 0, 0, 0);
    }
    
    GM_Vec4 viewSpace = inverseProjection * clipSpace;
    viewSpace.x /= viewSpace.w;
    viewSpace.y /= viewSpace.w;
    viewSpace.z /= viewSpace.w;

    return GM_Vec4(viewSpace.x, viewSpace.y, -1, 0);
}

GM_Vec3 MousePicker::toWorldSpace(GM_Vec4 viewSpace) {
    bool success = false;
    GM_Matrix4 inverseView = GM_Matrix4::inverse(this->view, &success);
    if (!success) {
        CKG_LOG_ERROR("Failed to invert view matrix\n");
        return GM_Vec3(0, 0, 0);
    }

    GM_Vec4 worldSpace = inverseView * viewSpace;
    return GM_Vec3(worldSpace.x, worldSpace.y, worldSpace.z);
}
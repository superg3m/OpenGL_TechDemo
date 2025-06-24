#include <MousePicker.hpp>
#include <IOD.hpp>
#include <GameState.hpp>

MousePicker::MousePicker() {
    this->rayDirection = GM_Vec3(0, 0, 0);
    this->rayOrigin = GM_Vec3(0, 0, 0);
}


// Date: June 20, 2025
// TODO(Jovanni): Fully break this down and study it more, then write up a summery of going from screen space to world space and vice versa
void MousePicker::update(GM_Matrix4 projection, GM_Matrix4 view) {
    float mouse_x = IOD::getMouseX();
    float mouse_y = IOD::getMouseY();

    GM_Vec2 ndc = GM_Vec2(
        ((2.0 * mouse_x) / GameState::WINDOW_WIDTH) - 1, 
        ((2.0 * mouse_y) / GameState::WINDOW_HEIGHT) - 1
    );

    GM_Vec4 clipCoordsNear = GM_Vec4(ndc.x, -ndc.y, -1.0f, 1.0f);
    GM_Vec4 clipCoordsFar  = GM_Vec4(ndc.x, -ndc.y,  1.0f, 1.0f);

    bool success = false;
    GM_Matrix4 ProjectView = projection * view;
    GM_Matrix4 viewProjectionInverse = GM_Matrix4::inverse(ProjectView, &success);
    if (!success) {
        CKG_LOG_ERROR("Failed to inverse proj view matrix\n");
    }

    // Unproject
    GM_Vec4 worldPosNear = viewProjectionInverse * clipCoordsNear;
    GM_Vec4 worldPosFar  = viewProjectionInverse * clipCoordsFar;

    // Perspective Divide (need to study this more it makes snse i'm just a little bit confused because it feels like it should be a multiply to undo the perspective divide?)
    this->rayOrigin = GM_Vec3(worldPosNear.x / worldPosNear.w,
                              worldPosNear.y / worldPosNear.w,
                              worldPosNear.z / worldPosNear.w);

    GM_Vec3 rayDirectionRaw = GM_Vec3(worldPosFar.x / worldPosFar.w,
                                      worldPosFar.y / worldPosFar.w,
                                      worldPosFar.z / worldPosFar.w);

    this->rayDirection = (rayDirectionRaw - this->rayOrigin).normalize();
}

GM_Vec3 MousePicker::getFromObjectZ(GM_Matrix4 projection, GM_Matrix4 view, float object_z) {
    bool success = false;
    GM_Matrix4 projectionInverse = GM_Matrix4::inverse(projection, &success);
    if (!success) {
        CKG_LOG_ERROR("Failed to inverse projection matrix\n");
    }

    GM_Matrix4 viewInverse = GM_Matrix4::inverse(view, &success);
    if (!success) {
        CKG_LOG_ERROR("Failed to inverse view matrix\n");
    }

    // Step 1 - Viewport to NDC
    float mouse_x = IOD::getMouseX();
    float mouse_y = IOD::getMouseY();

    GM_Vec2 ndc = GM_Vec2(
        ((2.0 * mouse_x) / GameState::WINDOW_WIDTH) - 1, 
        ((2.0 * mouse_y) / GameState::WINDOW_HEIGHT) - 1
    );
    
    float focal_length = 1.0f/tanf((float)DEGREES_TO_RAD(GameState::camera.zoom / 2.0f));
    float ar = (float)GameState::WINDOW_WIDTH / (float)GameState::WINDOW_HEIGHT;
    GM_Vec3 ray_view((-ndc.x * ar) / focal_length, ndc.y / focal_length, 1.0f);

    // Date: June 21, 2025
    // NOTE(Jovanni): So the way this works is pretty dang cool;
    // When you turn everything into viewspace you are actually offseting all the objects by putting your cameara at the origin;
    // so you need to take the view * objectPos to get the cannoncial/view pos;
    GM_Vec4 view_space_intersect = GM_Vec4(ray_view.scale(object_z), 1.0f);

    GM_Vec4 point_world = viewInverse * view_space_intersect;
    return GM_Vec3(point_world);
}
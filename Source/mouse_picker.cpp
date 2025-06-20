#include <mouse_picker.hpp>
#include <IOD.hpp>
#include <game.hpp>


// Date: June 20, 2025
// TODO(Jovanni): Fully break this down and study it more, then write up a summery of going from screen space to world space and vice versa
void MousePicker::update(GM_Matrix4 projection, GM_Matrix4 view) {
    float mouse_x = IOD::getMouseX();
    float mouse_y = IOD::getMouseY();

    GM_Vec2 ndc = GM_Vec2(
        ((2.0 * mouse_x) / Game::WINDOW_WIDTH) - 1, 
        ((2.0 * mouse_y) / Game::WINDOW_HEIGHT) - 1
    );

    GM_Vec4 clipCoordsNear = GM_Vec4(ndc.x, -ndc.y, -1.0f, 1.0f); // Z = -1 for near plane
    GM_Vec4 clipCoordsFar  = GM_Vec4(ndc.x, -ndc.y,  1.0f, 1.0f); // Z =  1 for far plane

    GM_Matrix4 ProjectView = projection * view;
    GM_Matrix4 viewProjectionInverse = GM_Matrix4::inverse(ProjectView, nullptr);

    // 4. Unproject the two points to World Space
    GM_Vec4 worldPosNear = viewProjectionInverse * clipCoordsNear;
    GM_Vec4 worldPosFar  = viewProjectionInverse * clipCoordsFar;

    // 5. Perform Perspective Divide for both points
    this->rayOrigin = GM_Vec3(worldPosNear.x / worldPosNear.w,
                              worldPosNear.y / worldPosNear.w,
                              worldPosNear.z / worldPosNear.w);

    GM_Vec3 rayDirectionRaw = GM_Vec3(worldPosFar.x / worldPosFar.w,
                                      worldPosFar.y / worldPosFar.w,
                                      worldPosFar.z / worldPosFar.w);

    this->rayDirection = (rayDirectionRaw - this->rayOrigin).normalize();
}
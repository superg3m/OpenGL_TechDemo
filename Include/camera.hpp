#pragma once

#include <gm.hpp>

enum CameraDirection {
    UP,
    DOWN,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float DEFAULTED_YAW         = -90.0f;
const float DEFAULTED_PITCH       =  0.0f;
const float DEFAULTED_SPEED       =  10.0f;
const float DEFAULTED_SENSITIVITY =  0.1f;
const float DEFAULTED_ZOOM        =  45.0f;

struct Camera {
    GM_Vec3 position;
    GM_Vec3 front;
    GM_Vec3 up;
    GM_Vec3 right;
    GM_Vec3 world_up;

    // euler Angles
    float yaw;
    float pitch;

    // camera options
    float mouse_sensitivity;
    float movement_speed;
    float zoom;

    Camera(GM_Vec3 position);
    Camera(float x, float y, float z);
    GM_Matrix4 get_view_matrix();
    GM_Matrix4 lookat(GM_Vec3 target_position);
    GM_Matrix4 lookat(float x, float y, float z);
    void process_keyboard(CameraDirection direction, float deltaTime);
    void process_mouse_movements(float xoffset, float yoffset, bool contrain_pitch = true);
    void process_mouse_scroll(float yoffset);
    
private:
    void update_camera_vectors();
};
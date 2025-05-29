#pragma once

#include <gm.h>

enum CameraDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float DEFAULTED_YAW         = -90.0f;
const float DEFAULTED_PITCH       =  0.0f;
const float DEFAULTED_SPEED       =  2.5f;
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
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    Camera(GM_Vec3 position);
    Camera(float x, float y, float z);

    GM_Matrix4 getViewMatrix() {
        return gm_look_at(this->position, gm_v3_add(this->position, this->front), this->up);
    }

    void process_keyboard(CameraDirection direction, float deltaTime) {
        float velocity = movement_speed * deltaTime;
        if (direction == FORWARD) {
            this->position = gm_v3_add(this->position, gm_v3_scale(this->front, velocity));
        } else if (direction == BACKWARD) {
            this->position = gm_v3_add(this->position, gm_v3_scale(this->front, -velocity));
        } else if (direction == LEFT) {
            this->position = gm_v3_add(this->position, gm_v3_scale(this->right, velocity));
        } else if (direction == RIGHT) {
            this->position = gm_v3_add(this->position, gm_v3_scale(this->right, -velocity));
        }
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool contrain_pitch = true) {
        xoffset *= this->mouse_sensitivity;
        yoffset *= this->mouse_sensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if (contrain_pitch) {
            if (Pitch > 89.0f) {
                Pitch = 89.0f;
            } else if (Pitch < -89.0f) {
                Pitch = -89.0f;
            }
        }


        update_camera_vectors();
    }

private:
    void update_camera_vectors();
};
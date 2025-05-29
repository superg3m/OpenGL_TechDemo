#include <camera.hpp>

void Camera::update_camera_vectors() {
    GM_Vec3 new_front;
    new_front.x = cosf(DEGREES_TO_RAD(this->yaw)) * cosf(DEGREES_TO_RAD(this->pitch));
    new_front.y = sinf(DEGREES_TO_RAD(this->pitch));
    new_front.z = sinf(DEGREES_TO_RAD(this->yaw)) * cosf(DEGREES_TO_RAD(this->pitch));
    this->front = gm_vec3_normalize(new_front);

    this->right = gm_vec3_normalize(gm_vec3_cross(this->front, this->world_up));
    this->up    = gm_vec3_normalize(gm_vec3_cross(this->right, this->front));
}

Camera::Camera(GM_Vec3 position) {
    this->position = position;

    this->world_up = gm_vec3_create(0, 1, 0);
    this->up = gm_vec3_create(0, 1, 0);

    this->mouse_sensitivity = DEFAULTED_SENSITIVITY;
    this->movement_speed = DEFAULTED_SPEED;
    this->zoom = DEFAULTED_ZOOM;
    this->yaw = DEFAULTED_YAW;
    this->pitch = DEFAULTED_PITCH;

    this->update_camera_vectors();
}

Camera::Camera(float x, float y, float z) {
    this->position = gm_vec3_create(x, y, z);

    this->world_up = gm_vec3_create(0, 1, 0);
    this->up = gm_vec3_create(0, 1, 0);

    this->mouse_sensitivity = DEFAULTED_SENSITIVITY;
    this->movement_speed = DEFAULTED_SPEED;
    this->zoom = DEFAULTED_ZOOM;
    this->yaw = DEFAULTED_YAW;
    this->pitch = DEFAULTED_PITCH;

    this->update_camera_vectors();
}


GM_Matrix4 Camera::look_at(GM_Vec3 target_position) {
    return gm_mat4_look_at(this->position, target_position, this->up);
}

GM_Matrix4 Camera::look_at(float x, float y, float z) {
    return gm_mat4_look_at(this->position, gm_vec3_create(x, y, z), this->up);
}

GM_Matrix4 Camera::get_view_matrix() {
    return gm_mat4_look_at(this->position, gm_vec3_add(this->position, this->front), this->up);
}

void Camera::process_keyboard(CameraDirection direction, float deltaTime) {
    float velocity = movement_speed * deltaTime;

    if (direction == UP) {
        this->position = gm_vec3_add(this->position, gm_vec3_scale(this->world_up, velocity));
    }

    if (direction == DOWN) {
        this->position = gm_vec3_add(this->position, gm_vec3_scale(this->world_up, -velocity));
    }

    if (direction == FORWARD) {
        this->position = gm_vec3_add(this->position, gm_vec3_scale(this->front, velocity));
    }
    
    if (direction == BACKWARD) {
        this->position = gm_vec3_add(this->position, gm_vec3_scale(this->front, -velocity));
    }
    
    if (direction == LEFT) {
        this->position = gm_vec3_add(this->position, gm_vec3_scale(this->right, -velocity));
    }
    
    if (direction == RIGHT) {
        this->position = gm_vec3_add(this->position, gm_vec3_scale(this->right, velocity));
    }
}

void Camera::process_mouse_movements(float xoffset, float yoffset, bool contrain_pitch) {
    xoffset *= this->mouse_sensitivity;
    yoffset *= this->mouse_sensitivity;

    this->yaw   += xoffset;
    this->pitch += yoffset;

    if (contrain_pitch) {
        if (this->pitch > 89.0f) {
            this->pitch = 89.0f;
        } else if (this->pitch < -89.0f) {
            this->pitch = -89.0f;
        }
    }

    this->update_camera_vectors();
}

void Camera::process_mouse_scroll(float yoffset) {
    this->zoom -= yoffset;
    if (this->zoom < 1.0f) {
        this->zoom = 1.0f;
    }

    if (this->zoom > 45.0f) {
        this->zoom = 45.0f;
    }
}
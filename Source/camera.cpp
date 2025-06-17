#include <camera.hpp>

void Camera::update_camera_vectors() {
    GM_Vec3 new_front = GM_Vec3::euler(this->yaw, this->pitch);
    this->front = new_front.normalize();
    this->right = GM_Vec3::cross(this->front, this->world_up).normalize();
    this->up    = GM_Vec3::cross(this->right, this->front).normalize();
}

Camera::Camera(GM_Vec3 position) {
    this->position = position;

    this->world_up = GM_Vec3(0, 1, 0);
    this->up = GM_Vec3(0, 1, 0);

    this->mouse_sensitivity = DEFAULTED_SENSITIVITY;
    this->movement_speed = DEFAULTED_SPEED;
    this->zoom = DEFAULTED_ZOOM;
    this->yaw = DEFAULTED_YAW;
    this->pitch = DEFAULTED_PITCH;

    this->update_camera_vectors();
}

Camera::Camera(float x, float y, float z) {
    this->position = GM_Vec3(x, y, z);

    this->world_up = GM_Vec3(0, 1, 0);
    this->up = GM_Vec3(0, 1, 0);

    this->mouse_sensitivity = DEFAULTED_SENSITIVITY;
    this->movement_speed = DEFAULTED_SPEED;
    this->zoom = DEFAULTED_ZOOM;
    this->yaw = DEFAULTED_YAW;
    this->pitch = DEFAULTED_PITCH;

    this->update_camera_vectors();
}


GM_Matrix4 Camera::lookat(GM_Vec3 target_position) {
    return GM_Matrix4::lookat(this->position, target_position, this->up);
}

GM_Matrix4 Camera::lookat(float x, float y, float z) {
    return GM_Matrix4::lookat(this->position, GM_Vec3(x, y, z), this->up);
}

GM_Matrix4 Camera::get_view_matrix() {
    return GM_Matrix4::lookat(this->position, this->position + this->front, this->up);
}

void Camera::process_keyboard(CameraDirection direction, float deltaTime) {
    float velocity = movement_speed * deltaTime;

    if (direction == UP) {
        this->position = this->position + this->world_up.scale(velocity);
    }

    if (direction == DOWN) {
        this->position = this->position + this->world_up.scale(-velocity);
    }

    if (direction == FORWARD) {
        this->position = this->position + this->front.scale(velocity);
    }
    
    if (direction == BACKWARD) {
        this->position = this->position + this->front.scale(-velocity);
    }
    
    if (direction == LEFT) {
        this->position = this->position + this->right.scale(-velocity);
    }
    
    if (direction == RIGHT) {
        this->position = this->position + this->right.scale(velocity);
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
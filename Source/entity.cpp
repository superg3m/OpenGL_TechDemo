#include <game.hpp>

Entity::Entity(EntityType type, Mesh mesh) {
    this->reference_identifer = Game::getReferenceID();
    this->type = type;

    this->position = GM_Vec3Lit(0, 0, 0);
    this->orientation = GM_QuaternionLit(1, 0, 0, 0);
    this->scale = GM_Vec3Lit(1, 1, 1);

    this->mesh = mesh;
}

void Entity::setPosition(GM_Vec3 position) {
    this->position = position;
}

void Entity::setPosition(float x, float y, float z) {
    this->position = GM_Vec3Lit(x, y, z);
}


void Entity::setOrientation(GM_Quaternion orientation) {
    this->orientation = orientation;
}

void Entity::setEulerAngles(GM_Vec3 euler) {
    this->orientation = gm_quat_from_euler(euler);
}

void Entity::setEulerAngles(float theta_x, float theta_y, float theta_z) {
    this->orientation = gm_quat_from_euler(GM_Vec3Lit(-theta_x, -theta_y, -theta_z));
}

void Entity::setScale(float scale) {
    this->scale = GM_Vec3Lit(scale, scale, scale);
}

void Entity::setScale(GM_Vec3 scale) {
    this->scale = scale;
}

void Entity::setScale(float scale_x, float scale_y, float scale_z) {
    this->scale = GM_Vec3Lit(scale_x, scale_y, scale_z);
}

GM_Matrix4 Entity::getTransform() {
    GM_Matrix4 transform = gm_mat4_identity();
    transform = gm_mat4_scale(transform, this->scale);

    float theta; GM_Vec3 axis;
    gm_quat_to_axis_angle(this->orientation, &theta, &axis);
    transform = gm_mat4_rotate(transform, theta, axis);
    transform = gm_mat4_translate(transform, this->position);

    return transform;
}

void Entity::draw() {
    this->mesh.draw(this->getTransform());
}
#include <game.hpp>

Entity::Entity(EntityType type, Mesh mesh) {
    this->reference_identifer = Game::getReferenceID();
    this->type = type;

    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::literal(1, 0, 0, 0);
    this->scale = GM_Vec3(1, 1, 1);

    this->mesh = mesh;
    this->dead = false;
}

Entity* Entity::Sprite(EntityType type) {
    Shader spirteShader = Shader({"../../shader_source/test.vert", "../../shader_source/test.frag"});
    Material spirteMaterial = Material(spirteShader);
    Mesh spirteMesh = Mesh(spirteMaterial, Geometry::Quad());

    return new Entity(type, spirteMesh);
}

void Entity::setPosition(GM_Vec3 position) {
    this->position = position;
}

void Entity::setPosition(float x, float y, float z) {
    this->position = GM_Vec3(x, y, z);
}


void Entity::setOrientation(GM_Quaternion orientation) {
    this->orientation = orientation;
}

void Entity::setEulerAngles(GM_Vec3 euler) {
    this->orientation = GM_Quaternion::fromEuler(euler);
}

void Entity::setEulerAngles(float theta_x, float theta_y, float theta_z) {
    this->orientation = GM_Quaternion::fromEuler(GM_Vec3(theta_x, theta_y, theta_z));
}

void Entity::setScale(float scale) {
    this->scale = GM_Vec3(scale, scale, scale);
}

void Entity::setScale(GM_Vec3 scale) {
    this->scale = scale;
}

void Entity::setScale(float scale_x, float scale_y, float scale_z) {
    this->scale = GM_Vec3(scale_x, scale_y, scale_z);
}

GM_Matrix4 Entity::getTransform() {
    GM_Matrix4 transform = GM_Matrix4::identity();
    transform = GM_Matrix4::scale(transform, this->scale);
    transform = GM_Matrix4::rotate(transform, this->orientation);
    transform = GM_Matrix4::translate(transform, this->position);

    return transform;
}

void Entity::draw() {
    if (this->dead) return;

    this->mesh.draw(this->getTransform());
}
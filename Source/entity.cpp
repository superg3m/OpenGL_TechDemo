#include <GameState.hpp>

/*
Entity::Entity(Mesh* mesh) {
    this->reference_identifer = GameState::getReferenceID();

    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::literal(1, 0, 0, 0);
    this->scale = GM_Vec3(1, 1, 1);

    this->mesh = mesh;
    this->should_render_aabb = false;

    this->dead = false;
}

GM_AABB Entity::getAABB() {
    // find_min from verticies
    return GM_AABB::fromCenterExtents(this->position, this->scale);
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

void Entity::setTexture(std::string texture_name, GLTextureID id) {
    Model* model = dynamic_cast<Model*>(this->mesh);
    if (model) {
        CKG_LOG_ERROR("Setting a texture on a model is invalid right now!\n");
        return;
    }
    
    this->mesh->material.textures[texture_name] = id;
}

GM_Matrix4 Entity::getTransform() {
    GM_Matrix4 transform = GM_Matrix4::identity();
    transform = GM_Matrix4::scale(transform, this->scale);
    transform = GM_Matrix4::rotate(transform, this->orientation);
    transform = GM_Matrix4::translate(transform, this->position);

    return transform;
}

GM_Matrix4 Entity::getAABBTransform() {
    GM_AABB aabb = this->getAABB();
    GM_Matrix4 transform = GM_Matrix4::identity();
    transform = GM_Matrix4::scale(transform, aabb.getExtents().scale(1.005f)); // scale to stop z-fighting
    transform = GM_Matrix4::rotate(transform, this->orientation);
    transform = GM_Matrix4::translate(transform, aabb.getCenter());

    return transform;
}

void Entity::draw(Shader &shader, bool should_draw_textures) {
    if (this->dead) return;

    this->mesh->draw(shader, should_draw_textures);
}
*/
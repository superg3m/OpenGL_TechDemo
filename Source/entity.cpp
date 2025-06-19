#include <game.hpp>

Entity::Entity(Mesh mesh) {
    this->reference_identifer = Game::getReferenceID();

    this->position = GM_Vec3(0, 0, 0);
    this->orientation = GM_Quaternion::literal(1, 0, 0, 0);
    this->scale = GM_Vec3(1, 1, 1);

    this->mesh = mesh;

    Shader aabbShader = Shader({"../../shader_source/aabb/aabb.vert", "../../shader_source/aabb/aabb.frag"});
    this->aabb_mesh = Mesh(Material(aabbShader), Geometry::AABB());
    this->aabb_mesh.material.color = GM_Vec4(0, 1, 0, 1);
    this->should_render_aabb = false;

    this->dead = false;
}

GM_AABB Entity::getAABB() {
    return GM_AABB::fromCenterExtents(this->position, this->scale.scale(0.5));
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

void Entity::setTexture(GLTextureID id, TextureType type){
    this->mesh.material.textures[type] = id;
}

GM_Matrix4 Entity::getTransform() {
    GM_Matrix4 transform = GM_Matrix4::identity();
    transform = GM_Matrix4::scale(transform, this->scale);
    transform = GM_Matrix4::rotate(transform, this->orientation);
    transform = GM_Matrix4::translate(transform, this->position);

    return transform;
}

void Entity::draw(GM_Matrix4 model, GM_Matrix4 view, GM_Matrix4 projection) {
    if (this->dead) return;

    this->mesh.material.shader.setVec4("color", this->mesh.material.color);
    this->aabb_mesh.material.shader.setVec4("color", this->aabb_mesh.material.color);

    this->mesh.draw(model, view, projection);
    if (this->should_render_aabb) {
        this->aabb_mesh.draw(model, view, projection);
    }
}
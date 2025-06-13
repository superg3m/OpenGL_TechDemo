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

Entity* Entity::Brick(int brick_type) {
    Entity* brick = Entity::Sprite(ENTITY_TYPE_BRICK);
    TextureAtlas* breaking_atlas = ResourceLoader::getTextureAtlas("Breaking");
    switch (brick_type) {
        case 0: {
            brick->dead = true;
        } break;

        case 1: {
            brick->setTexture(ResourceLoader::getTexture(SOLID_BRICK), TEXTURE_COLOR);
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(1, 1, 1));
            brick->maxHealth = -1;
        } break;

        case 2: {
            brick->setTexture(ResourceLoader::getTexture(NORMAL_BRICK), TEXTURE_COLOR);
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(0, 0, 1));
            brick->maxHealth = 1;
        } break;

        case 3: {
            brick->setTexture(ResourceLoader::getTexture(NORMAL_BRICK), TEXTURE_COLOR);
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(0, 1, 0));
            brick->maxHealth = 2;
        } break;
    
        case 4: {
            brick->setTexture(ResourceLoader::getTexture(NORMAL_BRICK), TEXTURE_COLOR);
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(1, 0, 0));
            brick->maxHealth = 5;
        } break;

        case 5: {
            brick->setTexture(ResourceLoader::getTexture(NORMAL_BRICK), TEXTURE_COLOR);
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(1, 0, 1));
            brick->maxHealth = 10;
        } break;

        case 6: {
            brick->setTexture(ResourceLoader::getTexture(CRATE), TEXTURE_COLOR);
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(1, 1, 1));
            brick->maxHealth = 1;
        } break;
    }

    brick->health = brick->maxHealth;

    return brick;
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

void Entity::setTexture(GLTextureID id, TextureType type) {
    this->mesh.material.textures[type] = id;
}

GM_Matrix4 Entity::getTransform() {
    GM_Matrix4 transform = GM_Matrix4::identity();
    transform = GM_Matrix4::scale(transform, this->scale);
    transform = GM_Matrix4::rotate(transform, this->orientation);
    transform = GM_Matrix4::translate(transform, this->position);

    return transform;
}

void Entity::updateBall(float dt) {
    this->position = this->position + this->velocity.scale(dt);
}

void Entity::updateBrick() {
    if (this->maxHealth == this->health) {
        return;
    }

    if (this->health == 0) {
        this->dead = true;
    }
    
    float health_percentage = (float)this->health / (float)this->maxHealth;
    TextureAtlas* breaking_atlas = ResourceLoader::getTextureAtlas("Breaking");
    if (health_percentage <= 0.20f) {
        this->setTexture(breaking_atlas->getTexture("break_5"), TEXTURE_DECAL);
    } else if(health_percentage <= 0.30f) {
        this->setTexture(breaking_atlas->getTexture("break_4"), TEXTURE_DECAL);
    } else if(health_percentage <= 0.40f) {
        this->setTexture(breaking_atlas->getTexture("break_3"), TEXTURE_DECAL);
    } else if(health_percentage <= 0.50f) {
        this->setTexture(breaking_atlas->getTexture("break_2"), TEXTURE_DECAL);
    } else if(health_percentage <= 0.60f) {
        this->setTexture(breaking_atlas->getTexture("break_1"), TEXTURE_DECAL);
    } else if(health_percentage <= 0.70f) {
        this->setTexture(breaking_atlas->getTexture("break_0"), TEXTURE_DECAL);
    }
}

void Entity::draw() {
    if (this->dead) return;

    this->mesh.draw(this->getTransform());
}
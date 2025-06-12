#include <gm.hpp>
#include <ckg.h>
#include <vector>
#include <mesh.hpp>

enum EntityType {
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_BALL,
    ENTITY_TYPE_BRICK,
    ENTITY_TYPE_POWERUP,
    ENTITY_TYPE_BACKGROUND,
};

// Date: June 03, 2025
// TODO(Jovanni): I think the reference ID is a unique ID for serialization?
// The reference_id is for pointing another entity at this reference in a stable way
// think of like a puzzle where you reference some type of trigger entity you would use the reference_id
struct Entity {
    u64 reference_identifer;
    EntityType type;

    GM_Vec3 position;
    GM_Vec3 velocity;
    GM_Quaternion orientation;
    GM_Vec3 scale;

    Mesh mesh;

    int health;
    int maxHealth;
    bool dead;

    Entity(EntityType type, Mesh mesh);
    static Entity* Sprite(EntityType type);
    static Entity* Brick(int brick_type);

    void setPosition(GM_Vec3 position);
    void setPosition(float x, float y, float z);

    void setOrientation(GM_Quaternion orientation);
    void setEulerAngles(GM_Vec3 euler);
    void setEulerAngles(float theta_x, float theta_y, float theta_z);

    void setScale(float scale);
    void setScale(GM_Vec3 scale);
    void setScale(float scale_x, float scale_y, float scale_z);

    void setTexture(GLTextureID id, TextureType type);
    void updateBrick();

    GM_Matrix4 getTransform();
    void draw();
};
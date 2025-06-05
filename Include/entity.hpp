#include <gm.h>
#include <mesh.hpp>

// Date: June 03, 2025
// TODO(Jovanni): I think the reference ID is a unique ID for serialization?
// The reference_id is for pointing another entity at this reference in a stable way
// think of like a puzzle where you reference some type of trigger entity you would use the reference_id
struct EntityID {
    u64 reference_id;
    u32 entity_index;
};

enum EntityType {
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_BALL,
    ENTITY_TYPE_BLOCK,
    ENTITY_TYPE_POWERUP,
};

struct Entity {
    // base
    EntityID identifer;
    EntityType type;

    GM_Vec3 position;
    GM_Quaternion orientation;
    GM_Vec3 scale;

    Mesh mesh;

    // gameplay
};
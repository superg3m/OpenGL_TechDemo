struct Transform {
    GM_Vec3 translation;
    Quaternion orientation;
    GM_Vec3 scale;
};

// Date: June 03, 2025
// TODO(Jovanni): I think the reference ID is a unique ID for serialization?
// The reference_id is for pointing another entity at this reference in a stable way
// think of like a puzzle where you reference some type of trigger entity you would use the reference_id
typedef struct EntityID {
    u64 reference_id;
    u32 entity_index;
} EntityID;

typedef enum EntityType {
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_DRAGON,
    ENTITY_TYPE_PRESSURE_PLATE,
    ENTITY_TYPE_DOOR,
    ENTITY_TYPE_LEVER,
    ENTITY_TYPE_TRAP,
    ENTITY_TYPE_SEQUENCE_CONTROLLER,
} EntityType;

typedef struct Entity {
    // base
    EntityID identifer;
    EntityType type;
    Transform transform;
    Mesh mesh;
    Material material;

    // gameplay
    Damage damage;
    StatusEffect status_effect;
    Resource resource;

    // puzzles
    Interactable interactable;
    Triggerable triggerable;
} Entity;
#include <ckg.h>

typedef unsigned int AssetID;
typedef unsigned int EntityID;

struct GameMemory {
    CKG_Arena* permanent_arena; // General-purpose permanent memory
    CKG_Arena* entity_arena;    // For entity structs
    CKG_Arena* asset_arena;     // Textures, models, sounds, etc.
    CKG_Arena* scratch_arena;   // Frame-local scratch (temporary, reset every tick/frame)
};

/*
GameState {
    static Entity* player; // is of type entity
    static Camera camera;
    static Mesh Skybox;

    std::vector<Entity*> opaque_entities;
    std::vector<Entity*> transparent_entities;

    GameState();
    EntityID createEntity(Mesh* mesh);
}
    */
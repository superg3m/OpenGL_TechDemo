#include <game.h>

struct GameState {
    RenderGroup* render_groups;
};

internal GameState game_state;

void game_init() {
    EntityID player = entity_create();
    TransformComponent* transform = component_transform_create(glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0));


}

// sound and input later
void game_update_and_render(double delta_time) {
    
}
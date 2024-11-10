#include <ECS.h>

internal u32 entity_counter;
internal u32 component_counter;

internal EntityID* entity_ids = NULLPTR;
internal ComponentBase*** component_groups[COMPONENT_COUNT];

EntityID entity_create() {
    EntityID ret = entity_counter++;
    ckit_vector_push(entity_ids, ret);

    for (int i = 0; i < COMPONENT_COUNT; i++) {
        ComponentBase*** entity_group = component_groups[i];
        ckit_vector_push(entity_group, NULLPTR);
    }

    return ret;
}

void add_component_to_entity(EntityID entity_id, ComponentBase* base) {
    ComponentBase** entity_component_list = component_groups[base->type][entity_id];
    ckit_vector_push(entity_component_list, base);
}


HealthComponent* component_health_create(int current_health, int max_health) {
    HealthComponent* ret = new HealthComponent;
    ret->base.id = component_counter++;
    ret->base.type = TRANSFORM_COMPONENT;

    ret->current_health = current_health;
    ret->max_health = max_health;

    return ret;
}

TransformComponent* component_transform_create(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    TransformComponent* ret = new TransformComponent;
    ret->base.id = component_counter++;
    ret->base.type = TRANSFORM_COMPONENT;

    ret->position = position;
    ret->rotation = rotation;
    ret->scale = scale;

    return ret;
}

ComponentID* get_components_from_entity(EntityID entity_id) {
    return NULLPTR;
}
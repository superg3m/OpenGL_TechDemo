#pragma once

#include <ckit.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef u32 EntityID;
typedef u32 ComponentID;

enum ComponentType {
    HEALTH_COMPONENT = 0,
    TRANSFORM_COMPONENT,
    COMPONENT_COUNT
};

typedef struct ComponentBase {
    ComponentID id;
    ComponentType type;
} ComponentBase;

typedef struct HealthComponent {
    ComponentBase base;
    int current_health;
    int max_health;
} HealthComponent;

typedef struct TransformComponent {
    ComponentBase base;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
} TransformComponent;

EntityID entity_create();

void add_component_to_entity(EntityID entity_id, ComponentBase* base);
HealthComponent* component_health_create(int current_health, int max_health);
TransformComponent* component_transform_create(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
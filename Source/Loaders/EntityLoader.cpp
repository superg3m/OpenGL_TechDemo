// skybox
// lights
// entities

#include <EntityLoader.hpp>

std::vector<std::string> EntityLoader::entity_keys;
std::map<std::string, Entity*> EntityLoader::entities;

std::vector<std::string> EntityLoader::skybox_keys;
std::map<std::string, Entity*> EntityLoader::skyboxes;

std::vector<std::string> EntityLoader::light_keys;
std::map<std::string, Entity*> EntityLoader::lights;

void EntityLoader::setEntityReference(std::string key, Entity* entity) {
    if (EntityLoader::entities.count(key)) {
        CKG_LOG_WARN("EntityLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    EntityLoader::entity_keys.push_back(key);
    EntityLoader::entities[key] = entity;
}

Entity* EntityLoader::getEntityReference(std::string key) {
    if (!EntityLoader::entities.count(key)) {
        CKG_LOG_ERROR("EntityLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
        return nullptr;
    }

    return EntityLoader::entities[key];
}

void EntityLoader::setSkyboxReference(std::string key, Entity* entity) {
    if (EntityLoader::skyboxes.count(key)) {
        CKG_LOG_WARN("EntityLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    EntityLoader::skybox_keys.push_back(key);
    EntityLoader::skyboxes[key] = entity;
}

Entity* EntityLoader::getSkyboxReference(std::string key) {
    if (!EntityLoader::skyboxes.count(key)) {
        CKG_LOG_ERROR("EntityLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
        return nullptr;
    }

    return EntityLoader::skyboxes[key];
}

void EntityLoader::setLightReference(std::string key, Entity* entity) {
    if (EntityLoader::lights.count(key)) {
        CKG_LOG_WARN("EntityLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    EntityLoader::light_keys.push_back(key);
    EntityLoader::lights[key] = entity;
}

Entity* EntityLoader::getLightReference(std::string key) {
    if (!EntityLoader::lights.count(key)) {
        CKG_LOG_ERROR("EntityLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
        return nullptr;
    }

    return EntityLoader::lights[key];
}
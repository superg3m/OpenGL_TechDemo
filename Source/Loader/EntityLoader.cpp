#include <EntityLoader.hpp>

std::vector<std::string> EntityLoader::entity_keys;
std::vector<std::string> EntityLoader::light_keys;
std::vector<std::string> EntityLoader::transparent_keys;

std::map<std::string, Entity*> EntityLoader::entities;

void EntityLoader::registerEntity(std::string key, Entity* entity) {
    if (EntityLoader::entities.count(key)) {
        CKG_LOG_WARN("EntityLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    EntityLoader::entity_keys.push_back(key);
    EntityLoader::entities[key] = entity;
}

void EntityLoader::registerLight(std::string key, Entity* entity) {
    if (EntityLoader::entities.count(key)) {
        CKG_LOG_WARN("EntityLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    EntityLoader::light_keys.push_back(key);
    EntityLoader::entities[key] = entity;
}

void EntityLoader::registerTransparentEntity(std::string key, Entity* entity) {
    if (EntityLoader::entities.count(key)) {
        CKG_LOG_WARN("EntityLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    EntityLoader::transparent_keys.push_back(key);
    EntityLoader::entities[key] = entity;
}

Entity* EntityLoader::getEntity(std::string key) {
    if (!EntityLoader::entities.count(key)) {
        CKG_LOG_ERROR("EntityLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
        return nullptr;
    }

    return EntityLoader::entities[key];
}
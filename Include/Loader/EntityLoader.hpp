#pragma once

#include <map>
#include <string>

#include <Entity.hpp>

struct EntityLoader {
    static std::vector<std::string> entity_keys;
    static std::vector<std::string> light_keys;
    static std::vector<std::string> transparent_keys;
    static std::map<std::string, Entity*> entities;

    static void registerEntity(std::string key, Entity* entity);
    static void registerLight(std::string key, Entity* entity);
    static void registerTransparentEntity(std::string key, Entity* entity);

    static Entity* getEntity(std::string key);
};
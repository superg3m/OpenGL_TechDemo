#pragma once

#include <map>
#include <string>

#include <Entity.hpp>

struct EntityLoader {
    static std::vector<std::string> skybox_keys;
    static std::map<std::string, Entity*> skyboxes;

    static std::vector<std::string> light_keys;
    static std::map<std::string, Entity*> lights;
    
    static std::vector<std::string> transparent_keys;
    static std::map<std::string, Entity*> transparent_entities;

    static std::vector<std::string> entity_keys;
    static std::map<std::string, Entity*> entities;

    static void registerTransparentEntity(std::string key, Entity* entity);
    static Entity* getTransparentEntity(std::string key);

    static void registerEntity(std::string key, Entity* entity);
    static Entity* getEntity(std::string key);

    static void registerSkybox(std::string key, Entity* entity);
    static Entity* getSkybox(std::string key);

    static void registerLight(std::string key, Entity* entity);
    static Entity* getLight(std::string key);
};
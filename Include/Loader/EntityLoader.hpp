#pragma once

#include <map>
#include <string>

#include <Entity.hpp>

struct EntityLoader {
    static std::vector<std::string> skybox_keys;
    static std::map<std::string, Entity*> skyboxes;

    static std::vector<std::string> light_keys;
    static std::map<std::string, Entity*> lights;

    static std::vector<std::string> entity_keys;
    static std::map<std::string, Entity*> entities;

    static void setEntityReference(std::string key, Entity* entity);
    static Entity* getEntityReference(std::string key);

    static void setSkyboxReference(std::string key, Entity* entity);
    static Entity* getSkyboxReference(std::string key);

    static void setLightReference(std::string key, Entity* entity);
    static Entity* getLightReference(std::string key);
};
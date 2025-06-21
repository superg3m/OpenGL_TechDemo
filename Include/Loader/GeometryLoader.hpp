#pragma once

#include <map>
#include <string>

#include <Geometry.hpp>

struct GeometryLoader {
    static std::vector<std::string> geometry_keys;
    static std::map<std::string, Geometry*> geometry;

    static void registerGeometry(std::string key, Entity* entity);
    static Entity* getEntityReference(std::string key);

    static void setSkyboxReference(std::string key, Entity* entity);
    static Entity* getSkyboxReference(std::string key);

    static void setLightReference(std::string key, Entity* entity);
    static Entity* getLightReference(std::string key);
};
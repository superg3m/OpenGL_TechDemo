#include <ckg.h>
#include <GeometryLoader.hpp>

std::vector<std::string> GeometryLoader::geometry_keys;
std::map<std::string, Geometry> GeometryLoader::geometry;

void GeometryLoader::registerGeometry(std::string key, Geometry entity) {
    if (GeometryLoader::geometry.count(key)) {
        CKG_LOG_WARN("EntityLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    GeometryLoader::geometry_keys.push_back(key);
    GeometryLoader::geometry[key] = entity;
}

Geometry GeometryLoader::getGeometry(std::string key) {
    if (!GeometryLoader::geometry.count(key)) {
        CKG_LOG_ERROR("GeometryLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
    }

    return GeometryLoader::geometry[key];
}
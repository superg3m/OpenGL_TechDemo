#pragma once

#include <map>
#include <string>
#include <Geometry.hpp>

struct GeometryLoader {
    static std::vector<std::string> geometry_keys;
    static std::map<std::string, Geometry> geometry;

    static void registerGeometry(std::string key, Geometry entity);
    static Geometry getGeometry(std::string key);
};
#pragma once

#include <map>
#include <string>
#include <Model.hpp>

struct ModelLoader {
    static std::vector<std::string> model_keys;
    static std::map<std::string, Model> models;

    static void registerModel(std::string key, Model model);
    static Model getModel(std::string key);
};
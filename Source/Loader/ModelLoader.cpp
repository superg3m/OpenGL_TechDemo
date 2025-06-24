#include <ckg.h>
#include <ModelLoader.hpp>

std::vector<std::string> ModelLoader::model_keys;
std::map<std::string, Model> ModelLoader::models;

void ModelLoader::registerModel(std::string key, Model model) {
    if (ModelLoader::models.count(key)) {
        CKG_LOG_WARN("ModelLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    ModelLoader::model_keys.push_back(key);
    ModelLoader::models[key] = model;
}

Model ModelLoader::getModel(std::string key) {
    if (!ModelLoader::models.count(key)) {
        CKG_LOG_ERROR("ModelLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
    }

    return ModelLoader::models[key];
}
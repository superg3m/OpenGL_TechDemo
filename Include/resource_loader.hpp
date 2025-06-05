#pragma once

#include <map>
#include <string>
#include <glad/glad.h>

using GLTextureID = int;
struct ResourceLoader {
    static std::map<std::string, GLTextureID> textures;
    static GLTextureID loadTexture(std::string name, const char *file, bool alpha, int texture_flags);
    static GLTextureID getTexture(std::string name);

private:
    ResourceLoader() {}
};
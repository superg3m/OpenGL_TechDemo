#pragma once

#include <map>
#include <string>
#include <glad/glad.h>

enum TextureFlag {
    TEXTURE_DEFAULT = 0x0,
    TEXTURE_PIXEL_PERFECT = 0x1,
    TEXTURE_VERTICAL_FLIP = 0x2
};

using GLTextureID = int;
struct ResourceLoader {
    static std::map<std::string, GLTextureID> textures;
    static GLTextureID loadTexture(std::string key, const char *file, int texture_flags = TEXTURE_DEFAULT);
    static GLTextureID getTexture(std::string key);

private:
    ResourceLoader() {}
};
#pragma once

#include <map>
#include <string>
#include <array>

#include <ckg.h>
#include <stb_image.h>

#include <glad/glad.h>

enum TextureFlag {
    TEXTURE_DEFAULT = 0x0,
    TEXTURE_PIXEL_PERFECT = 0x1,
    TEXTURE_VERTICAL_FLIP = 0x2
};

using GLTextureID = int;
struct TextureAtlas {
    u8* atlasTexture;
    int m_width;
    int m_height;
    GLenum format;
    std::map<std::string, GLTextureID> textures;

    TextureAtlas(std::string key, const char *file, int texture_flags);
    void bindPartitionedTexture(std::string key, int start_x, int start_y, int width, int height, int texture_flags = TEXTURE_DEFAULT);
    GLTextureID getTexture(std::string key);

    void freeAtlas();
};

struct TextureLoader {
    static std::map<std::string, GLTextureID> textures;
    static std::map<std::string, TextureAtlas*> atlas;

    static GLTextureID loadTexture(const char *file, int texture_flags = TEXTURE_DEFAULT, bool gammaCorrected = false);
    static GLTextureID loadTextureFromMemory(const u8* data, int width, int height, int nrChannels, int texture_flags = TEXTURE_DEFAULT, bool gammaCorrected = false);
    static void registerTexture(std::string key, const char *file, int texture_flags = TEXTURE_DEFAULT, bool gammaCorrected = false);
    static void registerTexture(std::string key, GLTextureID id);
    static void loadCubemapTexture(std::string key, std::array<const char*, 6> cubeMapTextures);
    static GLTextureID getTexture(std::string key);

    static TextureAtlas* loadTextureAtlas(std::string key, const char *file, int texture_flags = TEXTURE_DEFAULT);
    static TextureAtlas* getTextureAtlas(std::string key);
private:
    TextureLoader() = default;
};
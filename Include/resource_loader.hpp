#pragma once

#include <map>
#include <string>
#include <entity.hpp>

enum TextureFlag {
    TEXTURE_DEFAULT = 0x0,
    TEXTURE_PIXEL_PERFECT = 0x1,
    TEXTURE_VERTICAL_FLIP = 0x2
};

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

/**
 * @brief One natural Pattern that is being exhibited is the bundling of resources
 * For example I have a "BRICK" Texture and a "BRICK" Entity.
 */

using GLTextureID = int;
struct ResourceLoader {
    static std::map<std::string, GLTextureID> textures;
    static std::map<std::string, TextureAtlas*> atlas;
    static std::map<std::string, Geometry*> geometry;

    static std::vector<std::string> skybox_keys;
    static std::map<std::string, Entity*> skybox_map;

    static std::vector<std::string> light_keys;
    static std::map<std::string, Entity*> light_map;

    static std::vector<std::string> entity_keys;
    static std::map<std::string, Entity*> entity_map;

    static void loadTexture(std::string key, const char *file, int texture_flags = TEXTURE_DEFAULT);
    static void loadCubemapTexture(std::string key, std::array<const char*, 6> cubeMapTextures);
    static GLTextureID getTexture(std::string key);

    static TextureAtlas* loadTextureAtlas(std::string key, const char *file, int texture_flags = TEXTURE_DEFAULT);
    static TextureAtlas* getTextureAtlas(std::string key);

    static void setEntityReference(std::string key, Entity* entity);
    static Entity* getEntityReference(std::string key);

    static void setSkyboxReference(std::string key, Entity* entity);
    static Entity* getSkyboxReference(std::string key);

    static void setLightReference(std::string key, Entity* entity);
    static Entity* getLightReference(std::string key);
private:
    ResourceLoader() = default;
};
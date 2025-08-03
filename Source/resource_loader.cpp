#include <ckg.h>
#include <resource_loader.hpp>
#include <stb_image.h>

std::map<std::string, GLTextureID> ResourceLoader::textures;
std::map<std::string, TextureAtlas*> ResourceLoader::atlas;

std::vector<std::string> ResourceLoader::entity_keys;
std::map<std::string, Entity*> ResourceLoader::entity_map;


void ResourceLoader::loadTexture(std::string key, const char *file, int texture_flags) {
    ckg_assert_msg(ckg_io_path_exists(file), "Texture path: '%s' doesn't exist!\n", file);

    if (ResourceLoader::textures.count(key)) {
        CKG_LOG_WARN("ResourceLoader | Key: '%s' already exists\n", key.c_str());
        return;
    }

    GLenum MIPMAP_TYPE = GET_BIT(texture_flags, 0) ? GL_NEAREST : GL_LINEAR;
    GLenum TEXTURE_VERTICAL_FLIP = GET_BIT(texture_flags, 1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIPMAP_TYPE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MIPMAP_TYPE);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(TEXTURE_VERTICAL_FLIP);

    u8 *data = stbi_load(file, &width, &height, &nrChannels, 0);

    GLenum format = 0;
    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    } else {
        CKG_LOG_ERROR("ResourceLoader | Failed to pick a stb format, most likely related to assimp, try to link your libraries in a different order\n");
        ckg_assert(false);
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        CKG_LOG_ERROR("ResourceLoader | Failed to load texture\n");
    }

    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);

    ResourceLoader::textures[key] = texture;
}

GLTextureID ResourceLoader::getTexture(std::string key) {
    if (!ResourceLoader::textures.count(key)) {
        CKG_LOG_ERROR("ResourceLoader | Key: '%s' doesn't exists\n", key.c_str());
        return 0;
    }

    return ResourceLoader::textures[key];
}

TextureAtlas* ResourceLoader::loadTextureAtlas(std::string key, const char *file, int texture_flags) {
    ResourceLoader::atlas[key] = new TextureAtlas(key, file, texture_flags);

    return ResourceLoader::atlas[key];
}

TextureAtlas* ResourceLoader::getTextureAtlas(std::string key) {
    return ResourceLoader::atlas[key];
}

void ResourceLoader::setEntityReference(std::string key, Entity* entity) {
    if (ResourceLoader::entity_map.count(key)) {
        CKG_LOG_WARN("ResourceLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    ResourceLoader::entity_keys.push_back(key);
    ResourceLoader::entity_map[key] = entity;
}

Entity* ResourceLoader::getEntityReference(std::string key) {
    if (!ResourceLoader::entity_map.count(key)) {
        CKG_LOG_ERROR("ResourceLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
        return nullptr;
    }

    return ResourceLoader::entity_map[key];
}

TextureAtlas::TextureAtlas(std::string key, const char *file, int texture_flags) {
    ckg_assert_msg(ckg_io_path_exists(file), "Texture path: '%s' doesn't exist!\n", file);
    GLenum TEXTURE_VERTICAL_FLIP = GET_BIT(texture_flags, 1);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(TEXTURE_VERTICAL_FLIP);

    u8 *data = stbi_load(file, &width, &height, &nrChannels, 0);

    GLenum format = 0;
    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    } else {
        CKG_LOG_ERROR("ResourceLoader | Failed to pick a stb format, most likely related to assimp, try to link your libraries in a different order\n");
        ckg_assert(false);
    }

    if (data) {
        this->atlasTexture = data;
        this->m_height = height;
        this->m_width = width;
        this->format = format;


    } else {
        CKG_LOG_ERROR("ResourceLoader | Failed to load texture\n"); 
    }

    stbi_set_flip_vertically_on_load(false);
}

void TextureAtlas::bindPartitionedTexture(std::string key, int start_x, int start_y, int t_width, int t_height, int texture_flags) {
    GLenum MIPMAP_TYPE = GET_BIT(texture_flags, 0) ? GL_NEAREST : GL_LINEAR;
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIPMAP_TYPE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MIPMAP_TYPE);

    size_t bytes_per_pixel = 4;

    u8* partitioned_texture = (u8*)ckg_alloc((t_width * t_height) * bytes_per_pixel);
    u8* start_atlas = this->atlasTexture + ((start_x * bytes_per_pixel) + (this->m_width * bytes_per_pixel * start_y));

    for (int i = 0; i < t_height - 1; i++) {
        u8* partitioned_row = partitioned_texture + (t_width * bytes_per_pixel * i);
        u8* atlas_row = start_atlas + (this->m_width * bytes_per_pixel * i);

        ckg_memory_copy(partitioned_row, t_width * bytes_per_pixel, atlas_row, t_width * bytes_per_pixel);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, t_width, t_height, 0, format, GL_UNSIGNED_BYTE, partitioned_texture);
    glGenerateMipmap(GL_TEXTURE_2D);

    ckg_free(partitioned_texture);
    this->textures.emplace(key, texture);
}

GLTextureID TextureAtlas::getTexture(std::string key) {
    if (!this->textures.count(key)) {
        CKG_LOG_ERROR("ResourceLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
        return 0;
    }

    return this->textures[key];
}

void TextureAtlas::freeAtlas() {
    stbi_image_free(this->atlasTexture);
}
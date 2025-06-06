#include <ckg.h>
#include <resource_loader.hpp>
#include <stb_image.h>

std::map<std::string, int> ResourceLoader::textures;
std::map<std::string, Entity*> ResourceLoader::entities;

GLTextureID ResourceLoader::loadTexture(std::string key, const char *file, int texture_flags) {
    ckg_assert_msg(ckg_io_path_exists(file), "Texture path doesn't exist!\n");

    if (ResourceLoader::textures.count(key)) {
        CKG_LOG_WARN("ResourceLoader | Key: '%s' already exists loading cached TextureID\n", key.c_str());
        return ResourceLoader::getTexture(key);
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
        ckg_assert(FALSE);
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        CKG_LOG_ERROR("ResourceLoader | Failed to load texture\n");
    }

    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(FALSE);

    ResourceLoader::textures[key] = texture;

    return texture;
}

GLTextureID ResourceLoader::getTexture(std::string key) {
    if (!ResourceLoader::textures.count(key)) {
        CKG_LOG_ERROR("ResourceLoader | Key: '%s' doesn't exists\n", key.c_str());
        return 0;
    }

    return ResourceLoader::textures[key];
}


void ResourceLoader::setEntityReference(std::string key, Entity* entity) {
    if (ResourceLoader::entities.count(key)) {
        CKG_LOG_WARN("ResourceLoader | Key: '%s' already exists overwriting entity\n", key.c_str());
    }

    ResourceLoader::entities[key] = entity;
}

Entity* ResourceLoader::getEntityReference(std::string key) {
    if (!ResourceLoader::entities.count(key)) {
        CKG_LOG_ERROR("ResourceLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
        return nullptr;
    }

    return ResourceLoader::entities[key];
}
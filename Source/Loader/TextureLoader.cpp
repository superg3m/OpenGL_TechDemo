#include <TextureLoader.hpp>

std::map<std::string, GLTextureID> TextureLoader::textures;
std::map<std::string, TextureAtlas*> TextureLoader::atlas;

GLTextureID TextureLoader::loadTexture(const char *file, int texture_flags) {
    ckg_assert_msg(ckg_io_path_exists(file), "Texture path: '%s' doesn't exist!\n", file);

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
        CKG_LOG_ERROR("TextureLoader | Failed to pick a stb format, most likely related to assimp, try to link your libraries in a different order\n");
        ckg_assert(FALSE);
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        CKG_LOG_ERROR("TextureLoader | Failed to load texture\n");
    }

    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(FALSE);

    if (texture == 0) {
        CKG_LOG_ERROR("TextureLoader | id is invalid!\n");
    }

    return texture;
}

void TextureLoader::registerTexture(std::string key, const char *file, int texture_flags) {
    ckg_assert_msg(ckg_io_path_exists(file), "Texture path: '%s' doesn't exist!\n", file);

    if (TextureLoader::textures.count(key)) {
        CKG_LOG_WARN("TextureLoader | Key: '%s' already exists\n", key.c_str());
        return;
    }

    TextureLoader::textures[key] = TextureLoader::loadTexture(file, texture_flags);
}

void registerTexture(std::string key, GLTextureID id) {
    if (TextureLoader::textures.count(key)) {
        CKG_LOG_WARN("TextureLoader | Key: '%s' already exists\n", key.c_str());
        return;
    }

    TextureLoader::textures[key] = id;
}

// {right, left,  top, bottom, front, back}
void TextureLoader::loadCubemapTexture(std::string key, std::array<const char*, 6> cubeMapTextures) {
    if (TextureLoader::textures.count(key)) {
        CKG_LOG_WARN("TextureLoader | Key: '%s' already exists\n", key.c_str());
        return;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    unsigned char *data;  
    for(unsigned int i = 0; i < cubeMapTextures.size(); i++) {
        ckg_assert_msg(ckg_io_path_exists(cubeMapTextures[i]), "Texture path: '%s' doesn't exist!\n", cubeMapTextures[i]);
        int width, height, nrChannels;
        data = stbi_load(cubeMapTextures[i], &width, &height, &nrChannels, 0);
        
        GLenum format = 0;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        } else {
            CKG_LOG_ERROR("TextureLoader | Failed to pick a stb format, most likely related to assimp, try to link your libraries in a different order\n");
        }

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
            );
        } else {
            CKG_LOG_ERROR("TextureLoader | Failed to load texture\n");
        }

        stbi_image_free(data);
    }

    TextureLoader::textures[key] = texture;
}

GLTextureID TextureLoader::getTexture(std::string key) {
    if (!TextureLoader::textures.count(key)) {
        CKG_LOG_ERROR("TextureLoader | Key: '%s' doesn't exists\n", key.c_str());
        return 0;
    }

    return TextureLoader::textures[key];
}

TextureAtlas* TextureLoader::loadTextureAtlas(std::string key, const char *file, int texture_flags) {
    TextureLoader::atlas[key] = new TextureAtlas(key, file, texture_flags);

    return TextureLoader::atlas[key];
}

TextureAtlas* TextureLoader::getTextureAtlas(std::string key) {
    return TextureLoader::atlas[key];
}

TextureAtlas::TextureAtlas(std::string key, const char *file, int texture_flags) {
    ckg_assert_msg(ckg_io_path_exists(file), "Texture path: '%s' doesn't exist!\n", file);
    GLenum TEXTURE_VERTICAL_FLIP = GET_BIT(texture_flags, 1);

    unsigned int texture;
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
        CKG_LOG_ERROR("TextureLoader | Failed to pick a stb format, most likely related to assimp, try to link your libraries in a different order\n");
        ckg_assert(FALSE);
    }

    if (data) {
        this->atlasTexture = data;
        this->m_height = height;
        this->m_width = width;
        this->format = format;


    } else {
        CKG_LOG_ERROR("TextureLoader | Failed to load texture\n"); 
    }

    stbi_set_flip_vertically_on_load(FALSE);
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
        CKG_LOG_ERROR("TextureLoader | Key: '%s' doesn't exist for entity\n", key.c_str());
        return 0;
    }

    return this->textures[key];
}

void TextureAtlas::freeAtlas() {
    stbi_image_free(this->atlasTexture);
}
#include <ckg.h>
#include <Mesh.hpp>

Mesh::Mesh(Geometry geometry, Material material) {
    this->material = material;
    this->geometry = geometry;
}

void Mesh::draw(Shader &shader, bool should_draw_textures) {
    if (should_draw_textures) {
        for (const auto &[key, id] : this->material.textures) {
            shader.bindTexture(key.c_str(), id);
        }
    }

    if (this->material.opacity < 1.0f) {
        glEnable(GL_BLEND);
    }

    glBindVertexArray(geometry.VAO);
    if (geometry.index_count > 0) {
        glDrawElements(this->geometry.draw_type, geometry.index_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(this->geometry.draw_type, 0, geometry.vertex_count);
    }
    
    glDisable(GL_BLEND);
    glBindVertexArray(0);
    shader.unbindTextures();
}
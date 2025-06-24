#pragma once

#include <gm.hpp>
#include <ckg.h>

#include <vector>
#include <Mesh.hpp>

// Date: June 03, 2025
// TODO(Jovanni): I think the reference ID is a unique ID for serialization?
// The reference_id is for pointing another entity at this reference in a stable way
// think of like a puzzle where you reference some type of trigger entity you would use the reference_id

// Date: June 19, 2025
// TODO(Jovanni): aabb_mesh needs to be calculated by sorting vertrices and finding the min/max x, y, z
struct Entity {
    u64 reference_identifer;

    GM_Vec3 velocity;

    Mesh* mesh;
    bool should_render_aabb;

    float speed;
    int health;
    int maxHealth;
    bool dead;

    Entity(Mesh* mesh);

    GM_AABB getAABB();

    void setPosition(GM_Vec3 position);
    void setPosition(float x, float y, float z);

    void setOrientation(GM_Quaternion orientation);
    void setEulerAngles(GM_Vec3 euler);
    void setEulerAngles(float theta_x, float theta_y, float theta_z);

    void setScale(float scale);
    void setScale(GM_Vec3 scale);
    void setScale(float scale_x, float scale_y, float scale_z);

    void setTexture(std::string texture_name, GLTextureID id);
 
    GM_Matrix4 getTransform();
    GM_Matrix4 getAABBTransform();
    void draw(Shader &shader, bool should_draw_textures = true);
};
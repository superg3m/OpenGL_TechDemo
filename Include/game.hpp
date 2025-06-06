#pragma once

#include <gm.h>
#include <ckg.h>
#include <vector>
#include <mesh.hpp>
#include <resource_loader.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Date: June 03, 2025
// TODO(Jovanni): I think the reference ID is a unique ID for serialization?
// The reference_id is for pointing another entity at this reference in a stable way
// think of like a puzzle where you reference some type of trigger entity you would use the reference_id
struct EntityID {
    u64 reference_id;
    u32 entity_index;

    EntityID();
};

enum EntityType {
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_BALL,
    ENTITY_TYPE_BLOCK,
    ENTITY_TYPE_POWERUP,
};

struct Entity {
    // base
    EntityID identifer;
    EntityType type;

    GM_Vec3 position;
    GM_Quaternion orientation;
    GM_Vec3 scale;

    Mesh mesh;

    Entity(EntityType type, Mesh mesh);

    void setPosition(GM_Vec3 position);
    void setPosition(float x, float y, float z);

    void setOrientation(GM_Quaternion orientation);
    void setEulerAngles(GM_Vec3 euler);
    void setEulerAngles(float theta_x, float theta_y, float theta_z);

    void setScale(float scale);
    void setScale(GM_Vec3 scale);
    void setScale(float scale_x, float scale_y, float scale_z);

    GM_Matrix4 getTransform();
    void draw();
};

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
}; 

struct Game {
    GameState state;
  
    bool Keys[1024];
    static std::vector<Entity> entities;
    static unsigned int WINDOW_WIDTH;
    static unsigned int WINDOW_HEIGHT;
    Game(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

    GLFWwindow* initalizeWindow();
    void initializeResources();
    void initializeProjection();
    void initalizeEntities();

    static u64 getReferenceID();

    void processInput(GLFWwindow* window, float dt);
    void update(float dt);
    void render();
};
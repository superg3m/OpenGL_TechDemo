#pragma once

#include <gm.h>
#include <ckg.h>
#include <vector>
#include <mesh.hpp>
#include <resource_loader.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
}; 

struct Game {
    GameState state;
  
    bool Keys[1024];
    static GM_Matrix4 projection;
    static unsigned int WINDOW_WIDTH;
    static unsigned int WINDOW_HEIGHT;
    Game(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

    GLFWwindow* initalizeWindow();
    void initalizeResources();

    static u64 getReferenceID();

    void update(float dt);
    void render();
private:
    GM_Matrix4 getProjectionMatrix();
};
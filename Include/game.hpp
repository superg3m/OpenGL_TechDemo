#pragma once

#include <gm.hpp>
#include <ckg.h>
#include <vector>
#include <mesh.hpp>
#include <resource_loader.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ctime>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

enum GameState {
    GAME_ACTIVE,
    GAME_INACTIVE,
    GAME_MENU,
    GAME_WIN
}; 

#define PLAYER_PADDLE "PlayerPaddle"
#define SOLID_BRICK "SolidBrick"
#define NORMAL_BRICK "Brick"
#define BACKGROUND "Background"
#define CRATE "Crate"
#define BALL "Ball"

struct GameLevel {
    int cells_per_row;
    int cells_per_column;
    std::vector<int> level_data;
    std::vector<Entity*> brick_entity_references;

    GameLevel() = default;
    GameLevel(int cells_per_row, int cells_per_column, std::vector<int> level_data);
    void update();
};

struct Game {
    static GameState state;
    static GameLevel level;
  
    bool Keys[1024];
    static GM_Matrix4 projection;
    static unsigned int WINDOW_WIDTH;
    static unsigned int WINDOW_HEIGHT;
    Game(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

    GLFWwindow* initalizeWindow();
    void initalizeResources();

    static u64 getReferenceID();

    void update(GLFWwindow* window, float dt);
    void render();
private:
    GM_Matrix4 getProjectionMatrix();
};
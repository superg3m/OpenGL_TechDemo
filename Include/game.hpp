#pragma once

#include <gm.h>
#include <ckg.h>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
}; 

struct Game {
    GameState state;
  
    bool         Keys[1024];
    unsigned int WINDOW_WIDTH, WINDOW_HEIGHT;
    Game(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);
    ~Game();

    void initializeResources();
    void initializeProjection();

    void processInput(float dt);
    void update(float dt);
    void render();
};
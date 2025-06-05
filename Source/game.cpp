#include <game.hpp>

Game::Game(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHTs) { 
    this->state = GAME_ACTIVE;
    this->WINDOW_WIDTH = WINDOW_WIDTH;
    this->WINDOW_HEIGHT = WINDOW_HEIGHT;
}

void Game::initializeResources() {
    // loadTextures
}

void Game::initializeProjection() {
    float aspect_ratio = (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT;
    float l = 0.0f;
    float r = (float)this->WINDOW_WIDTH;
    float b = (float)this->WINDOW_HEIGHT;
    float t = 0.0f;
    float near_plane = -1.0f;
    float far_plane = 1.0f;
    GM_Matrix4 projection = gm_mat4_orthographic(l, r, b, t, near_plane, far_plane);
}

void Game::update(float dt) {
    
}

void Game::processInput(float dt) {
   
}

void Game::render() {
    
}


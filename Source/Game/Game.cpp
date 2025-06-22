#include <GameState.hpp>


// Date: June 22, 2025
// NOTE(Jovanni): Only run this one on engine init
__declspec(dllexport) void game_init(GameState* state) {
    // registerShader
    // registerTextures
    // registerModels

    // set engine framerate
    // engine->setFrameRate(120); // THis will set update and render to 60 frames per second
    // engine->setFrameRate(0); // if its zero then run as fast as you can
}

// Date: June 22, 2025
// TODO(Jovanni): Shaders need to recompile on hot reload
// put shader source in Game directory so I can file watch them wiht c_build.

// Not sure yet how this will work?
// might be a callback or it will be a binding system I really don't know yet?
// I know its useful to have both so IOD::getInputState(IOD_KEY_A) == IOD_State::PRESSED
__declspec(dllexport) void game_input(GameState* state) {

}

// Date: June 22, 2025
// TODO(Jovanni): Make this run at a fixed 1/60 dt or 60 frames per second
// this also has substeping
__declspec(dllexport) void game_physics_update(GameState* state, float dt) {

}

__declspec(dllexport) void game_update(GameState* state, float dt) {

}

__declspec(dllexport) void game_render(GameState* state) {

}
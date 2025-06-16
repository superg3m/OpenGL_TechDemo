#include <command.hpp>

std::map<InputCode, std::pair<InputState, BaseCommand*>> Input::input_behaviour;
std::map<InputCode, InputState> Input::input_state;

void Input::bind(InputCode input_code, InputState state, BaseCommand* command) {
    Input::input_behaviour[input_code] = std::make_pair(state, command);
}


std::unordered_map<int, InputCode> glfw_to_input_code = {
    {GLFW_KEY_A, KEY_A},
    {GLFW_KEY_B, KEY_B},
    {GLFW_KEY_C, KEY_C},
    {GLFW_KEY_D, KEY_D},
    {GLFW_KEY_E, KEY_E},
    {GLFW_KEY_F, KEY_F},
    {GLFW_KEY_G, KEY_G},
    {GLFW_KEY_H, KEY_H},
    {GLFW_KEY_I, KEY_I},
    {GLFW_KEY_J, KEY_J},
    {GLFW_KEY_K, KEY_K},
    {GLFW_KEY_L, KEY_L},
    {GLFW_KEY_M, KEY_M},
    {GLFW_KEY_N, KEY_N},
    {GLFW_KEY_O, KEY_O},
    {GLFW_KEY_P, KEY_P},
    {GLFW_KEY_Q, KEY_Q},
    {GLFW_KEY_R, KEY_R},
    {GLFW_KEY_S, KEY_S},
    {GLFW_KEY_T, KEY_T},
    {GLFW_KEY_U, KEY_U},
    {GLFW_KEY_V, KEY_V},
    {GLFW_KEY_W, KEY_W},
    {GLFW_KEY_X, KEY_X},
    {GLFW_KEY_Y, KEY_Y},
    {GLFW_KEY_Z, KEY_Z},

    // Numbers
    {GLFW_KEY_0, KEY_0},
    {GLFW_KEY_1, KEY_1},
    {GLFW_KEY_2, KEY_2},
    {GLFW_KEY_3, KEY_3},
    {GLFW_KEY_4, KEY_4},
    {GLFW_KEY_5, KEY_5},
    {GLFW_KEY_6, KEY_6},
    {GLFW_KEY_7, KEY_7},
    {GLFW_KEY_8, KEY_8},
    {GLFW_KEY_9, KEY_9},

    // Common control keys
    {GLFW_KEY_SPACE, KEY_SPACE},
    {GLFW_KEY_ENTER, KEY_ENTER},
    {GLFW_KEY_ESCAPE, KEY_ESCAPE},
    {GLFW_KEY_TAB, KEY_TAB},
    {GLFW_KEY_BACKSPACE, KEY_BACKSPACE},

    // Arrow keys
    {GLFW_KEY_LEFT, KEY_LEFT},
    {GLFW_KEY_RIGHT, KEY_RIGHT},
    {GLFW_KEY_UP, KEY_UP},
    {GLFW_KEY_DOWN, KEY_DOWN},

    // Modifier keys
    {GLFW_KEY_LEFT_CONTROL, KEY_CTRL},
    {GLFW_KEY_RIGHT_CONTROL, KEY_CTRL},
    {GLFW_KEY_LEFT_SHIFT, KEY_SHIFT},
    {GLFW_KEY_RIGHT_SHIFT, KEY_SHIFT},
    {GLFW_KEY_LEFT_ALT, KEY_ALT},
    {GLFW_KEY_RIGHT_ALT, KEY_ALT},

    // Function keys
    {GLFW_KEY_F1, KEY_F1},
    {GLFW_KEY_F2, KEY_F2},
    {GLFW_KEY_F3, KEY_F3},
    {GLFW_KEY_F4, KEY_F4},
    {GLFW_KEY_F5, KEY_F5},
    {GLFW_KEY_F6, KEY_F6},
    {GLFW_KEY_F7, KEY_F7},
    {GLFW_KEY_F8, KEY_F8},
    {GLFW_KEY_F9, KEY_F9},
    {GLFW_KEY_F10, KEY_F10},
    {GLFW_KEY_F11, KEY_F11},
    {GLFW_KEY_F12, KEY_F12},

    // Mouse buttons
    {GLFW_MOUSE_BUTTON_LEFT, MOUSE_BUTTON_LEFT},
    {GLFW_MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_RIGHT},
    {GLFW_MOUSE_BUTTON_MIDDLE, MOUSE_BUTTON_MIDDLE}
};

void Input::handleInput(GLFWwindow* window) {
    for (const auto& [glfw_code, input_code] : glfw_to_input_code) {
        int state;

        if (glfw_code >= GLFW_MOUSE_BUTTON_1 && glfw_code <= GLFW_MOUSE_BUTTON_LAST) {
            state = glfwGetMouseButton(window, glfw_code);
        } else {
            state = glfwGetKey(window, glfw_code);
        }

        if (state == GLFW_PRESS) {
            if (Input::input_state[input_code] == InputState::UP) {
                Input::input_state[input_code] = InputState::PRESSED;
            } else {
                Input::input_state[input_code] = InputState::DOWN;
            }
        } else {
            if (Input::input_state[input_code] == InputState::DOWN) {
                Input::input_state[input_code] = InputState::RELEASED;
            } else {
                Input::input_state[input_code] = InputState::UP;
            }
        }
    }

    for (const auto& [input_button, pair] : Input::input_behaviour) {
        const auto& [state, command] = pair;

        if (state & Input::input_state[input_button]) {
            command->execute();
        }
    }
}
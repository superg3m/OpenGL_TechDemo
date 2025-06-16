
#pragma once
#include <map>
#include <utility>
#include <functional>
#include <GLFW/glfw3.h>

/*
enum class EntityCommandType { 
    JUMP,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
};

struct EntityCommand : public BaseCommand {
    EntityCommand(Entity* entity, EntityCommandType type) {
        this->entity = entity;
        this->type = type;
    };
    ~EntityCommand() = default;
    void execute() override {
        switch (this->type) {
            case EntityCommandType::MOVE_LEFT: {
                this->entity.moveLeft();
            } break;

            case EntityCommandType::MOVE_LEFT: {
                this->entity.moveRight();
            } break;

            case EntityCommandType::JUMP: {
                this->entity.jump();
            } break;
        }
        this->entity
    };

    void undo() override {};
private:
    Entity* entity;
    EntityCommandType type;
};
*/

struct BaseCommand {
    virtual ~BaseCommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

struct LambdaCommand : public BaseCommand {
    std::function<void()> m_execute_fn;
    std::function<void()> m_undo_fn; // optional

    LambdaCommand(std::function<void()> execute_fn, std::function<void()> undo_fn = nullptr) {
        this->m_execute_fn = execute_fn;
        this->m_undo_fn = undo_fn;
    }

    void execute() override {
        if (this->m_execute_fn) {
            this->m_execute_fn();
        }
    }

    void undo() override {
        if (this->m_undo_fn) {
            this->m_undo_fn();
        }
    }
};

enum class InputState : uint8_t {
    UP       = 0x1,
    PRESSED  = 0x2,
    DOWN     = 0x4,
    RELEASED = 0x8
};

inline InputState operator|(InputState lhs, InputState rhs) {
    return static_cast<InputState>(
        static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
    );
}

inline uint8_t operator&(InputState lhs, InputState rhs) {
    return static_cast<uint8_t>(
        static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
    );
}

enum InputCode : int {
    // Letters (ASCII uppercase)
    KEY_A = 'A',
    KEY_B = 'B',
    KEY_C = 'C',
    KEY_D = 'D',
    KEY_E = 'E',
    KEY_F = 'F',
    KEY_G = 'G',
    KEY_H = 'H',
    KEY_I = 'I',
    KEY_J = 'J',
    KEY_K = 'K',
    KEY_L = 'L',
    KEY_M = 'M',
    KEY_N = 'N',
    KEY_O = 'O',
    KEY_P = 'P',
    KEY_Q = 'Q',
    KEY_R = 'R',
    KEY_S = 'S',
    KEY_T = 'T',
    KEY_U = 'U',
    KEY_V = 'V',
    KEY_W = 'W',
    KEY_X = 'X',
    KEY_Y = 'Y',
    KEY_Z = 'Z',

    // Numbers (optional, ASCII '0' - '9')
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',

    // Common control keys
    KEY_SPACE       = 32,
    KEY_ENTER       = 13,
    KEY_ESCAPE      = 27,
    KEY_TAB         = 9,
    KEY_BACKSPACE   = 8,

    // Arrow keys (starting from 128 to avoid conflict with ASCII)
    KEY_LEFT        = 128,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,

    // Modifier keys
    KEY_CTRL,
    KEY_SHIFT,
    KEY_ALT,

    // Function keys (optional)
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,

    // Mouse buttons (assigned values > 200 to avoid overlap)
    MOUSE_BUTTON_LEFT = 200,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE
};

struct Input {
    static std::map<InputCode, std::pair<InputState, BaseCommand*>> input_behaviour;
    static std::map<InputCode, InputState> input_state;

    static void handleInput(GLFWwindow* window);
    static void bind(InputCode input_code, InputState state, BaseCommand* command);
private:

};


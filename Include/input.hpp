
enum InputState {
    INPUT_UP,        // Key has been up for two frames or more
    INPUT_PRESSED,   // Just pressed this frame
    INPUT_DOWN,      // Held down
    INPUT_RELEASED   // Released this frame
};

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
    static BaseCommand* input_behaviour[256];
    static InputState input_state[256];


    static void bind(InputCode input_code, InputState state, BaseCommand* command);

    // Methods to bind commands...

private:

};

void Input::bind(InputCode input_code, InputState state, BaseCommand* command) {
    Input::input_behaviour[input_code] = command;
    Input::input_state[input_code] = state;
}
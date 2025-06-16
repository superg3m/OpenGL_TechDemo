#include <main.hpp>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Date: June 06, 2025
// TODO(Jovanni): I think resource loader should also load geometry because
// you can resuse a VAO instead of having to make an bind new ones all the time
// this would be especially good with Geometry::Cube() or Geometry::Sphere becuaes I don't have to regenerate the vertex data

void processInput(GLFWwindow* window);

int main() {
    Game application(800, 600);
    GLFWwindow* window = application.initalizeWindow();
    ckg_assert_msg(window, "failed to initalize glfw or glad\n");

    application.initalizeResources();

    Input::bind(MOUSE_BUTTON_LEFT, InputState::PRESSED, new LambdaCommand(
        []() {
            Game::state = GAME_ACTIVE;
        }
    ));

    Input::bind(KEY_UP, InputState::PRESSED|InputState::DOWN, new LambdaCommand(
        []() {
            Game::timeScale += 0.1f;
            CKG_LOG_DEBUG("%f\n", Game::timeScale);
        }
    ));

    Input::bind(KEY_DOWN, InputState::PRESSED|InputState::DOWN, new LambdaCommand(
        []() {
            Game::timeScale = MAX(Game::timeScale - 0.1f, 0.0f);
            CKG_LOG_DEBUG("%f\n", Game::timeScale);
        }
    ));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = (currentFrame - lastFrame) * Game::timeScale;
        lastFrame = currentFrame;

        Input::handleInput(window);

        int substeps = 8;
        float substep_dt = deltaTime / (float)substeps;
        for (int step = 0; step < substeps; step++) {
            application.update(window, substep_dt);
        }
        application.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Date: June 16, 2025
// TODO(Jovanni): Polling has the chance to miss input if frames are low.
// probably switch to some event-based system in the future.
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        Game::timeScale += 0.1f;
        CKG_LOG_DEBUG("%f\n", Game::timeScale);
    }
    
    if (glfwGetKey(window, GLFW_KEY_DOWN)== GLFW_PRESS) {
        Game::timeScale -= 0.1f;
        if (Game::timeScale < 0.0f) {
            Game::timeScale = 0.0f;
        }

        CKG_LOG_DEBUG("%f\n", Game::timeScale);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if ((key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        Game::mouse_captured = !Game::mouse_captured;
        glfwSetInputMode(window, GLFW_CURSOR, Game::mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        Game::timeScale = 0.0f;
    } else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        Game::timeScale = 1.0f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Game::WINDOW_WIDTH = width;
    Game::WINDOW_HEIGHT = height;

    Entity* background = ResourceLoader::getEntityReference(BACKGROUND);
    background->setPosition((float)Game::WINDOW_WIDTH / 2.0f, (float)Game::WINDOW_HEIGHT / 2.0f, 1);
    background->setScale((float)Game::WINDOW_WIDTH, (float)Game::WINDOW_HEIGHT, 1);

    Entity* paddle = ResourceLoader::getEntityReference(PLAYER_PADDLE);
    paddle->setScale((float)Game::WINDOW_WIDTH / 6.0f, (float)Game::WINDOW_HEIGHT / 20.0f, 1);

    Game::level.update();
    
    glViewport(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
}
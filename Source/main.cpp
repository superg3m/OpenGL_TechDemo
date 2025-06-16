#include <main.hpp>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Date: June 06, 2025
// TODO(Jovanni): I think resource loader should also load geometry because
// you can resuse a VAO instead of having to make an bind new ones all the time
// this would be especially good with Geometry::Cube() or Geometry::Sphere becuaes I don't have to regenerate the vertex data

int main() {
    Game application(800, 600);
    GLFWwindow* window = application.initalizeWindow();
    ckg_assert_msg(window, "failed to initalize glfw or glad\n");

    application.initalizeResources();
    IOD_GLFW_Setup(window);

    IOD_Profile* profile = IOD::createProfile("player");
    profile->bind(IOD_MOUSE_BUTTON_LEFT, IOD_InputState::RELEASED,
        []() {
            Game::state = GAME_ACTIVE;
        }
    );

    profile->bind(IOD_KEY_ESCAPE, IOD_InputState::PRESSED,
        [&]() {
            glfwSetWindowShouldClose(window, true);
        }
    );

    profile->bind(IOD_KEY_UP, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            Game::timeScale += 0.1f;
            CKG_LOG_DEBUG("%f\n", Game::timeScale);
        }
    );

    profile->bind(IOD_KEY_DOWN, IOD_InputState::PRESSED|IOD_InputState::DOWN, 
        []() {
            Game::timeScale = MAX(Game::timeScale - 0.1f, 0.0f);
            CKG_LOG_DEBUG("%f\n", Game::timeScale);
        }
    );

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = (currentFrame - lastFrame) * Game::timeScale;
        lastFrame = currentFrame;

        IOD::poll();

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
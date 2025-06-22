#include <Main.hpp>

float lastFrame = 0.0f;

// Date: June 06, 2025
// TODO(Jovanni): I think resource loader should also load geometry because
// you can resuse a VAO instead of having to make an bind new ones all the time
// this would be especially good with Geometry::Cube() or Geometry::Sphere becuaes I don't have to regenerate the vertex data

int main() {
    GameState application(800, 800);
    GLFWwindow* window = application.initalizeWindow();
    ckg_assert_msg(window, "failed to initalize glfw or glad\n");

    IOD_GLFW_SETUP(window);
    application.initalizeResources();
    application.initalizeInputBindings();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        GameState::deltaTime = (currentFrame - lastFrame) * GameState::timeScale;
        lastFrame = currentFrame;

        IOD::poll();

        application.update(window, GameState::deltaTime);
        application.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    GameState::WINDOW_WIDTH = width;
    GameState::WINDOW_HEIGHT = height;
    
    glViewport(0, 0, GameState::WINDOW_WIDTH, GameState::WINDOW_HEIGHT);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    GameState::camera.process_mouse_scroll((float)yoffset);
}
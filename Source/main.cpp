#include <main.hpp>

float lastFrame = 0.0f;

// Date: June 06, 2025
// TODO(Jovanni): I think resource loader should also load geometry because
// you can resuse a VAO instead of having to make an bind new ones all the time
// this would be especially good with Geometry::Cube() or Geometry::Sphere becuaes I don't have to regenerate the vertex data

int main() {
    Game application(800, 600);
    GLFWwindow* window = application.initalizeWindow();
    ckg_assert_msg(window, "failed to initalize glfw or glad\n");

    IOD_GLFW_SETUP(window);
    application.initalizeResources();
    application.initalizeInputBindings();

    MousePicker picker = MousePicker();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        Game::deltaTime = (currentFrame - lastFrame) * Game::timeScale;
        lastFrame = currentFrame;

        IOD::poll();

        int substeps = 8;
        float substep_dt = Game::deltaTime / (float)substeps;
        for (int step = 0; step < substeps; step++) {
            application.update(window, substep_dt);
        }

        if (!Game::mouse_captured) {
            picker.update(application.getProjectionMatrix(), Game::camera.get_view_matrix());
            CKG_LOG_SUCCESS("(%f, %f, %f)\n", picker.ray.x, picker.ray.y, picker.ray.z);
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
    
    glViewport(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Game::camera.process_mouse_scroll((float)yoffset);
}
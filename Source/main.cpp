#include <main.hpp>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    GLFWwindow* window = initalize_glfw_and_glad();
    ckg_assert_msg(window, "failed to initalize glfw or glad");

    Game application(WINDOW_WIDTH, WINDOW_HEIGHT);
    application.initializeResources();
    application.initializeProjection();

    Shader cubeShader({"../../shader_source/model.vert", "../../shader_source/model.frag"});
    cubeShader.addTexture("../../assets/container.jpg", "texture1", TEXTURE_VERTICAL_FLIP);
    Mesh cubeMesh(Material(&cubeShader), Geometry({3, 3, 2}, vertices));
    GM_Vec3 cube_position = GM_Vec3Lit(-0.5, -0.5, -0.5);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
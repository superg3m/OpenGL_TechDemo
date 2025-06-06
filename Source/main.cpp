#include <main.hpp>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    GLFWwindow* window = initalize_glfw_and_glad();
    ckg_assert_msg(window, "failed to initalize glfw or glad");

    Game application(WINDOW_WIDTH, WINDOW_HEIGHT);
    application.initializeResources();
    application.initializeProjection();

    Shader cubeShader({"../../shader_source/no_projection.vert", "../../shader_source/no_projection.frag"});
    Material cubeMaterial = Material(&cubeShader);
    cubeMaterial.textures[TEXTURE_COLOR] = ResourceLoader::getTexture("container");
    cubeMaterial.textures[TEXTURE_DECAL] = ResourceLoader::getTexture("smiley_face");
    Mesh cubeMesh = Mesh(cubeMaterial, Geometry::Quad());
    Entity player = Entity(ENTITY_TYPE_PLAYER, cubeMesh);
    Game::entities.push_back(player);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        application.render();
    
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
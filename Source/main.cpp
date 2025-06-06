#include <main.hpp>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    Game application(800, 600);
    GLFWwindow* window = application.initalizeWindow();
    ckg_assert_msg(window, "failed to initalize glfw or glad\n");

    application.initializeResources();
    
    Shader cubeShader({"../../shader_source/no_projection.vert", "../../shader_source/no_projection.frag"});
    Material cubeMaterial = Material(&cubeShader);
    cubeMaterial.textures[TEXTURE_COLOR] = ResourceLoader::getTexture("container");
    cubeMaterial.textures[TEXTURE_DECAL] = ResourceLoader::getTexture("smiley_face");
    Mesh cubeMesh = Mesh(cubeMaterial, Geometry::Quad());
    Entity player = Entity(ENTITY_TYPE_PLAYER, cubeMesh);
    Game::entities.push_back(player);

    application.initializeProjection();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        application.processInput(window, deltaTime);

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

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
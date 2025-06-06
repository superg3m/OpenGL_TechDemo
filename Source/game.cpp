#include <game.hpp>

GM_Matrix4 Game::projection;
unsigned int Game::WINDOW_WIDTH;
unsigned int Game::WINDOW_HEIGHT;

Game::Game(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT) { 
    this->state = GAME_ACTIVE;
    Game::WINDOW_WIDTH = WINDOW_WIDTH;
    Game::WINDOW_HEIGHT = WINDOW_HEIGHT;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

GLFWwindow* Game::initalizeWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        CKG_LOG_ERROR("Failed to create GLFW window\n");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        CKG_LOG_ERROR("Failed to initialize GLAD\n");
        glfwTerminate();
        return nullptr;
    }

    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    return window;
}

GM_Matrix4 Game::getProjectionMatrix() {
    float aspect_ratio = (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT;
    float l = 0.0f; 
    float r = (float)this->WINDOW_WIDTH; 
    float b = (float)this->WINDOW_HEIGHT;
    float t = 0.0f; 
    float near_plane = -1.0f; 
    float far_plane = 1.0f;

    return gm_mat4_orthographic(l, r, b, t, near_plane, far_plane);
}

u64 Game::getReferenceID() {
    local_persist u64 referenceID = 0;
    return referenceID++;
}

void Game::initializeResources() {
    ResourceLoader::loadTexture("container", "../../assets/container.jpg");
    ResourceLoader::loadTexture("smiley_face", "../../assets/awesomeface.png");
}

void Game::initalizeEntities() {
    Shader cubeShader({"../../shader_source/test.vert", "../../shader_source/test.frag"});
    Material cubeMaterial = Material(cubeShader);
    cubeMaterial.textures[TEXTURE_COLOR] = ResourceLoader::getTexture("smiley_face");
    Mesh cubeMesh = Mesh(cubeMaterial, Geometry::Quad());
    Entity* player = new Entity(ENTITY_TYPE_PLAYER, cubeMesh);

    player->setScale(300.0f, 400.0f, 1.0f);
    player->setPosition(200.0f, 200.0f, 0.0f);

    ResourceLoader::setEntityReference("player", player);
}

void Game::update(float dt) {
    local_persist float currentTime = 0; currentTime += dt;

    GM_Matrix4 projection = this->getProjectionMatrix();

    for (const auto& pair : ResourceLoader::entities) {
        Entity* entity = pair.second;
        entity->mesh.material.shader.setMat4("projection", projection);
    }

    Entity* player = ResourceLoader::getEntityReference("player");
    player->setEulerAngles(0.0f, 0.0f, sinf(currentTime) * 90.0f);
}

void Game::processInput(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    local_persist bool mouse_captured = true;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && mouse_captured) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !mouse_captured) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouse_captured = true;
    }
}

void Game::render() {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    for (const auto& pair : ResourceLoader::entities) {
        Entity* entity = pair.second;
        entity->draw();
    }
}


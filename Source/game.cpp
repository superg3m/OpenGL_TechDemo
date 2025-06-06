#include <game.hpp>

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

void Game::initalizeEntities() {
    Shader cubeShader({"../../shader_source/test.vert", "../../shader_source/test.frag"});
    Material cubeMaterial = Material(cubeShader);
    cubeMaterial.textures[TEXTURE_COLOR] = ResourceLoader::getTexture("smiley_face");
    Mesh cubeMesh = Mesh(cubeMaterial, Geometry::Quad());
    Entity* player = new Entity(ENTITY_TYPE_PLAYER, cubeMesh);
    player->setScale(1, 1.25f, 1);
    ResourceLoader::setEntityReference("player", player);
}

void Game::initializeResources() {
    ResourceLoader::loadTexture("container", "../../assets/container.jpg");
    ResourceLoader::loadTexture("smiley_face", "../../assets/awesomeface.png", TEXTURE_VERTICAL_FLIP);
}

void Game::initializeProjection() {
    float aspect_ratio = (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT;
    float l = 0.0f;
    float r = (float)this->WINDOW_WIDTH;
    float b = (float)this->WINDOW_HEIGHT;
    float t = 0.0f;
    float near_plane = -1.0f;
    float far_plane = 1.0f;
    GM_Matrix4 projection = gm_mat4_orthographic(l, r, b, t, near_plane, far_plane);
}

u64 Game::getReferenceID() {
    local_persist u64 referenceID = 0;
    return referenceID++;
}

void Game::update(float dt) {
    local_persist float currentTime = 0; currentTime += dt;

    Entity* player = ResourceLoader::getEntityReference("player");
    player->setEulerAngles(0.0f, 0.0f, sinf(currentTime) * 90.0f);
}

void Game::processInput(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Game::render() {
    for (std::map<std::string, Entity*>::iterator it = ResourceLoader::entities.begin(); it != ResourceLoader::entities.end(); it++) {
        it->second->draw();
    }
}


#include <game.hpp>

std::vector<Entity> Game::entities;
unsigned int Game::WINDOW_WIDTH;
unsigned int Game::WINDOW_HEIGHT;

EntityID::EntityID() {
    // Date: June 05, 2025
    // TODO(Jovanni): Eventually have a system where it check for the next available entity index
    this->entity_index = Game::entities.size();
    this->reference_id = Game::getReferenceID();
}

Entity::Entity(EntityType type, Mesh mesh) {
    this->identifer = EntityID();
    this->type = type;

    this->position = GM_Vec3Lit(0, 0, 0);
    this->orientation = GM_QuaternionLit(1, 0, 0, 0);
    this->scale = GM_Vec3Lit(1, 1, 1);

    this->mesh = mesh;
}

void Entity::setPosition(GM_Vec3 position) {
    this->position = position;
}

void Entity::setPosition(float x, float y, float z) {
    this->position = GM_Vec3Lit(x, y, z);
}


void Entity::setOrientation(GM_Quaternion orientation) {
    this->orientation = orientation;
}

void Entity::setEulerAngles(GM_Vec3 euler) {
    this->orientation = gm_quat_from_euler(euler);
}

void Entity::setEulerAngles(float theta_x, float theta_y, float theta_z) {
    this->orientation = gm_quat_from_euler(GM_Vec3Lit(theta_x, theta_y, theta_z));
}

void Entity::setScale(float scale) {
    this->scale = GM_Vec3Lit(scale, scale, scale);
}

void Entity::setScale(GM_Vec3 scale) {
    this->scale = scale;
}

void Entity::setScale(float scale_x, float scale_y, float scale_z) {
    this->scale = GM_Vec3Lit(scale_x, scale_y, scale_z);
}

GM_Matrix4 Entity::getTransform() {
    GM_Matrix4 transform = gm_mat4_identity();
    transform = gm_mat4_scale(transform, this->scale);
    transform = gm_quat_to_mat4(this->orientation);
    transform = gm_mat4_translate(transform, this->position);

    return transform;
}

void Entity::draw() {
    this->mesh.draw(this->getTransform());
}

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

void Game:: initalizeEntities() {
    // Shader cubeShader({"../../shader_source/no_projection.vert", "../../shader_source/no_projection.frag"});
    // Material cubeMaterial = Material(&cubeShader);
    // cubeMaterial.textures[TEXTURE_COLOR] = ResourceLoader::getTexture("container");
    // cubeMaterial.textures[TEXTURE_DECAL] = ResourceLoader::getTexture("smiley_face");
    // Mesh cubeMesh = Mesh(cubeMaterial, Geometry::Quad());
    // Entity player = Entity(ENTITY_TYPE_PLAYER, cubeMesh);
    // Game::entities.push_back(player);

    // if I put it in here it goes out of scope so I think I will have to allocate these or use a static array of elements
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
    
}

void Game::processInput(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Game::render() {
    for (int i = 0; i < Game::entities.size(); i++) {
        Entity e = Game::entities[i];
        e.draw();
    }
}


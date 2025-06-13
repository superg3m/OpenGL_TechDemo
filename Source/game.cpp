#include <game.hpp>

GM_Matrix4 Game::projection;
unsigned int Game::WINDOW_WIDTH;
unsigned int Game::WINDOW_HEIGHT;
GameLevel Game::level;

Game::Game(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT) { 
    this->state = GAME_ACTIVE;
    Game::WINDOW_WIDTH = WINDOW_WIDTH;
    Game::WINDOW_HEIGHT = WINDOW_HEIGHT;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        CKG_LOG_ERROR("Failed to initialize GLAD\n");
        glfwTerminate();
        return nullptr;
    }

    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_DEPTH_TEST);

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

    return GM_Matrix4::orthographic(l, r, b, t, near_plane, far_plane);
}

u64 Game::getReferenceID() {
    local_persist u64 referenceID = 0;
    return referenceID++;
}

void create_brick(GameLevel* level, float grid_cell_width, float grid_cell_hieght, float &x_offset, float &y_offset, int brick_type) {
    // Date: June 11, 2025
    // TODO(Jovanni): I hate the fact that you have to declare all of this
    Entity* brick = Entity::Brick(brick_type);
    
    local_persist int brick_index = 0;
    brick->setPosition(GM_Vec3(x_offset, y_offset, 0));
    brick->setScale(GM_Vec3(grid_cell_width, grid_cell_hieght, 1));
    x_offset += grid_cell_width;
    if (x_offset >= Game::WINDOW_WIDTH) {
        x_offset = grid_cell_width / 2;
        y_offset += grid_cell_hieght;
    }

    ResourceLoader::setEntityReference("brick_" + std::to_string(brick_index), brick);
    level->brick_entity_references.push_back(brick);
    brick_index++;
}


GameLevel::GameLevel(int cells_per_row, int cells_per_column, std::vector<int> level_data) {
    this->cells_per_row = cells_per_row;
    this->cells_per_column = cells_per_column;

    float cell_width = Game::WINDOW_WIDTH / (float)this->cells_per_row;
    float cell_height = (Game::WINDOW_HEIGHT / 2.0f) / (float)this->cells_per_column;

    float x_offset = (cell_width / 2.0f);
    float y_offset = (cell_height / 2.0f);

    for (int brick : level_data) {
        create_brick(this, cell_width, cell_height, x_offset, y_offset, brick);
    }
}

void GameLevel::update() {
    float cell_width = Game::WINDOW_WIDTH / (float)this->cells_per_row;
    float cell_height = (Game::WINDOW_HEIGHT / 2.0f) / (float)this->cells_per_column;
    float x_offset = (cell_width / 2.0f);
    float y_offset = (cell_height / 2.0f);

    for (Entity* brick : this->brick_entity_references) {
        brick->setPosition(GM_Vec3(x_offset, y_offset, 0));
        brick->setScale(GM_Vec3(cell_width, cell_height, 1));

        x_offset += cell_width;
        if (x_offset >= Game::WINDOW_WIDTH) {
            x_offset = cell_width / 2;
            y_offset += cell_height;
        }
    }
}


void Game::initalizeResources() {
    srand(time(0));

    TextureAtlas* atlas = ResourceLoader::loadTextureAtlas("Breaking", "../../assets/block_breaking_atlas.png");
    atlas->bindPartitionedTexture("break_5", 0,   0,   396, 408, TEXTURE_PIXEL_PERFECT);
    atlas->bindPartitionedTexture("break_4", 396, 0,   396, 408, TEXTURE_PIXEL_PERFECT);
    atlas->bindPartitionedTexture("break_3", 0,   408, 396, 408, TEXTURE_PIXEL_PERFECT);
    atlas->bindPartitionedTexture("break_2", 396, 408, 396, 408, TEXTURE_PIXEL_PERFECT);
    atlas->bindPartitionedTexture("break_1", 0,   816, 396, 408, TEXTURE_PIXEL_PERFECT);
    atlas->bindPartitionedTexture("break_0", 396, 816, 396, 408, TEXTURE_PIXEL_PERFECT);
    atlas->freeAtlas();

    ResourceLoader::loadTexture(BACKGROUND, "../../assets/background.jpg");
    ResourceLoader::loadTexture(PLAYER_PADDLE, "../../assets/paddle.png");
    ResourceLoader::loadTexture(CRATE, "../../assets/container.jpg");
    ResourceLoader::loadTexture(NORMAL_BRICK, "../../assets/block.png");
    ResourceLoader::loadTexture(SOLID_BRICK, "../../assets/block_solid.png");

    std::vector<int> brick_layout = {
        1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 1, 3, 1, 4, 1, 5, 1, 4, 1, 3, 1, 2,
        2, 3, 3, 4, 4, 5, 6, 5, 4, 4, 3, 3, 2,
        2, 1, 3, 1, 4, 1, 5, 1, 4, 1, 3, 1, 2,
        2, 2, 3, 3, 4, 4, 5, 4, 4, 3, 3, 2, 2,
    };

    // Date: June 11, 2025
    // TODO(Jovanni): Maybe make the notion of just a sprite becuase its pretty annoying to do this all...
    Entity* background = Entity::Sprite(ENTITY_TYPE_BACKGROUND);
    background->setPosition(GM_Vec3(Game::WINDOW_WIDTH / 2.0f, Game::WINDOW_HEIGHT / 2.0f, 0));
    background->setScale((float)Game::WINDOW_WIDTH, (float)Game::WINDOW_HEIGHT, 1);
    background->setTexture(ResourceLoader::getTexture(BACKGROUND), TEXTURE_COLOR);
    background->mesh.material.shader.setVec3("spriteColor", GM_Vec3(1, 1, 1));
    ResourceLoader::setEntityReference(BACKGROUND, background);

    Entity* player = Entity::Sprite(ENTITY_TYPE_PLAYER);
    player->setPosition(GM_Vec3(Game::WINDOW_WIDTH / 2.0f, Game::WINDOW_HEIGHT / 1.15f, 0));
    player->setScale((float)Game::WINDOW_WIDTH / 6.0f, (float)Game::WINDOW_HEIGHT / 32.0f, 1);
    player->setTexture(ResourceLoader::getTexture(PLAYER_PADDLE), TEXTURE_COLOR);
    player->mesh.material.shader.setVec3("spriteColor", GM_Vec3(1, 1, 1));
    ResourceLoader::setEntityReference(PLAYER_PADDLE, player);

    Entity* ball = Entity::Sprite(ENTITY_TYPE_BALL);
    ball->setPosition(GM_Vec3(Game::WINDOW_WIDTH / 2.0f, (Game::WINDOW_HEIGHT / 1.15f) - (Game::WINDOW_WIDTH / 40.0f), 0));
    ball->setScale(Game::WINDOW_WIDTH / 40.0f, Game::WINDOW_WIDTH / 40.0f, 1);
    ball->setTexture(ResourceLoader::getTexture(NORMAL_BRICK), TEXTURE_COLOR);
    ball->mesh.material.shader.setVec3("spriteColor", GM_Vec3(1, 1, 1));
    ResourceLoader::setEntityReference(BALL, ball);

    Game::level = GameLevel(13, 6, brick_layout);
}

float currentTime = 0;
bool ball_is_in_play = false;
void Game::update(GLFWwindow* window, float dt) {
    currentTime += dt;

    // Date: June 12, 2025
    // TODO(Jovanni): Get the mouse delta to get calculate the velocity in the x direction
    // so i can move the ball in the x direction

    // gm_move_towards 1 velocity

    double mouse_x = 0;
    glfwGetCursorPos(window, &mouse_x, nullptr);
    mouse_x = CLAMP(mouse_x, 0, Game::WINDOW_WIDTH);
    Entity* player_paddle = ResourceLoader::getEntityReference(PLAYER_PADDLE);
    player_paddle->setPosition(mouse_x, Game::WINDOW_HEIGHT / 1.15f, 0);

    Entity* ball = ResourceLoader::getEntityReference(BALL);
    if (!ball_is_in_play) {
        ball->setPosition(mouse_x, (Game::WINDOW_HEIGHT / 1.15f) - ball->scale.x, 0);
    }
    //ball->updateBall(dt);
    // collisionDetectionHere
    
    int special_break_index = rand() % Game::level.brick_entity_references.size();
    for (int i = 0; i < Game::level.brick_entity_references.size(); i++) {
        Entity* entity = Game::level.brick_entity_references[i];
        if (entity->maxHealth == -1 || entity->dead) continue;

        if (special_break_index == i) {
            entity->health -= 1;
        }

        entity->updateBrick();
    }
}

void Game::render() {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    GM_Matrix4 projection = this->getProjectionMatrix();
    for (const auto& key : ResourceLoader::entity_keys) {
        Entity* entity = ResourceLoader::getEntityReference(key);
        entity->mesh.material.shader.setMat4("projection", projection);
        entity->draw();
    }
}


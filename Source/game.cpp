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

    return GM_Matrix4::orthographic(l, r, b, t, near_plane, far_plane);
}

u64 Game::getReferenceID() {
    local_persist u64 referenceID = 0;
    return referenceID++;
}

void create_brick(GameLevel* level, float grid_cell_width, float grid_cell_hieght, float &x_offset, float &y_offset, int brick_type) {
    // Date: June 11, 2025
    // TODO(Jovanni): I hate the fact that you have to declare all of this
    Shader brickShader = Shader({"../../shader_source/test.vert", "../../shader_source/test.frag"});
    Material brickMaterial = Material(brickShader);
    Mesh brickMesh = Mesh(brickMaterial, Geometry::Quad());
    Entity* brick = new Entity(ENTITY_TYPE_BRICK, brickMesh);

    local_persist int brick_index = 0;
    brick->setPosition(GM_Vec3(x_offset, y_offset, 0));
    brick->setScale(GM_Vec3(grid_cell_width, grid_cell_hieght, 1));
    x_offset += grid_cell_width;
    if (x_offset >= Game::WINDOW_WIDTH) {
        x_offset = grid_cell_width / 2;
        y_offset += grid_cell_hieght;
    }

    TextureAtlas* breaking_atlas = ResourceLoader::getTextureAtlas("breaking");
    switch (brick_type) {
        case 0: {

        } break;

        case 1: {
            // Date: June 11, 2025
            // TODO(Jovanni): I hate that you have to do brick->mesh.material
            // should just be brick->setTexture(ResourceLoader::getTexture("SolidBrick"), TEXTURE_COLOR);
            // should just be brick->setVec3("spriteColor", GM_Vec3(1, 1, 1));
            brick->mesh.material.textures[TEXTURE_COLOR] = ResourceLoader::getTexture("SolidBrick");
            brick->mesh.material.textures[TEXTURE_DECAL] = breaking_atlas->getTexture("break_4");
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(1, 1, 1));
        } break;

        case 2: {
            brick->mesh.material.textures[TEXTURE_COLOR] = ResourceLoader::getTexture("Brick");
            brick->mesh.material.textures[TEXTURE_DECAL] = breaking_atlas->getTexture("break_4");
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(0, 0, 1));
            brick->health = 1;
        } break;

        case 3: {
            brick->mesh.material.textures[TEXTURE_COLOR] = ResourceLoader::getTexture("Brick");
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(0, 1, 0));
            brick->health = 2;
        } break;
    
        case 4: {
            brick->mesh.material.textures[TEXTURE_COLOR] = ResourceLoader::getTexture("Brick");
            brick->mesh.material.shader.setVec3("spriteColor", GM_Vec3(1, 0, 0));
            brick->health = 5;
        } break;
    }
    
    ResourceLoader::setEntityReference("brick_" + std::to_string(brick_index), brick);
    level->brick_entity_references.push_back(brick);
    brick_index++;
}


GameLevel::GameLevel(int cells_per_row, int cells_per_column, std::vector<int> level_data) {
    this->cells_per_row = cells_per_row;
    this->cells_per_column = cells_per_column;

    float cell_width = Game::WINDOW_WIDTH / (float)this->cells_per_row;
    float cell_height =  Game::WINDOW_HEIGHT / (float)this->cells_per_column;

    float x_offset = (cell_width / 2.0f);
    float y_offset = (cell_height / 2.0f);

    for (int brick : level_data) {
        create_brick(this, cell_width, cell_height, x_offset, y_offset, brick);
    }
}

void GameLevel::update() {
    float cell_width = Game::WINDOW_WIDTH / (float)this->cells_per_row;
    float cell_height =  Game::WINDOW_HEIGHT / (float)this->cells_per_column;
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
    TextureAtlas* atlas = ResourceLoader::loadTextureAtlas("breaking", "../../assets/block_breaking_atlas.png");
    atlas->bindPartitionedTexture("break_4", 0, 0, 396, 408, TEXTURE_PIXEL_PERFECT);
    atlas->freeAtlas();
    // atlas->bindPartitionedTexture("break_1");
    // atlas->bindPartitionedTexture("break_2");
    // atlas->bindPartitionedTexture("break_3");

    ResourceLoader::loadTexture("container", "../../assets/container.jpg");
    ResourceLoader::loadTexture("Brick", "../../assets/block.png");
    ResourceLoader::loadTexture("SolidBrick", "../../assets/block_solid.png");

    std::vector<int> brick_layout = {
        1, 1, 1, 1, 1, 1, 
        2, 2, 0, 0, 2, 2,
        3, 3, 4, 4, 3, 3
    };

    Game::level = GameLevel(6, 6, brick_layout);
}

static float currentTime = 0;
void Game::update(float dt) {
    currentTime += dt;
}

void Game::render() {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    GM_Matrix4 projection = this->getProjectionMatrix();
    for (const auto& pair : ResourceLoader::entities) {
        Entity* entity = pair.second;
        entity->mesh.material.shader.setMat4("projection", projection);
        entity->draw();
    }
}


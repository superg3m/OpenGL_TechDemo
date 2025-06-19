#include <game.hpp>

GM_Matrix4 Game::projection;
unsigned int Game::WINDOW_WIDTH;
unsigned int Game::WINDOW_HEIGHT;
bool Game::mouse_captured = true;
float Game::timeScale = 1.0f;
Camera Game::camera = Camera(0, 0, 5);
float Game::deltaTime = 0.0f;
MousePicker Game::picker = MousePicker();

Game::Game(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT) { 
    Game::WINDOW_WIDTH = WINDOW_WIDTH;
    Game::WINDOW_HEIGHT = WINDOW_HEIGHT;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    IOD_GLFW_BIND_MOUSE_MOVE_CALLBACK([](GLFWwindow *window, double xpos, double ypos) {
        float mouse_x = xpos;
        float mouse_y = ypos;
        local_persist float last_mouse_x = mouse_x;
        local_persist float last_mouse_y = mouse_y;

        float xoffset = mouse_x - last_mouse_x;
        float yoffset = last_mouse_y - mouse_y;

        last_mouse_x = mouse_x;
        last_mouse_y = mouse_y;

        if (Game::mouse_captured) {
            Game::camera.process_mouse_movements(xoffset, yoffset);
        }
    });

    IOD_GLFW_BIND_KEY_CALLBACK([](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_C && action == GLFW_PRESS) {
            Game::mouse_captured = !Game::mouse_captured;
            glfwSetInputMode(window, GLFW_CURSOR, Game::mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
    });
    
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
    float fov = Game::camera.zoom;
    float aspect = (float)Game::WINDOW_WIDTH / (float)Game::WINDOW_HEIGHT;
    float near_plane = 0.1f;
    float far_plane = 100.0f;

    return GM_Matrix4::perspective(fov, aspect, near_plane, far_plane);
}

u64 Game::getReferenceID() {
    local_persist u64 referenceID = 0;
    return referenceID++;
}

void Game::initalizeResources() {
    srand(time(0));

    std::array<const char*, 6> cubemap_faces = {
        "../../assets/skybox/right.jpg",
        "../../assets/skybox/left.jpg",
        "../../assets/skybox/top.jpg",
        "../../assets/skybox/bottom.jpg",
        "../../assets/skybox/front.jpg",
        "../../assets/skybox/back.jpg",
    };

    ResourceLoader::loadCubemapTexture(SKYBOX, cubemap_faces);
    ResourceLoader::loadTexture(CRATE, "../../assets/container.jpg");

    // Date: June 11, 2025
    // TODO(Jovanni): Maybe make the notion of just a sprite becuase its pretty annoying to do this all...

    Shader skyboxShader = Shader({"../../shader_source/skybox/skybox.vert", "../../shader_source/skybox/skybox.frag"});
    Material skyboxMaterial = Material(skyboxShader);
    Entity* skybox = new Entity(Mesh(skyboxMaterial, Geometry::Cube()));
    skybox->setTexture(ResourceLoader::getTexture(SKYBOX), TEXTURE_CUBEMAP);
    ResourceLoader::setEntityReference(SKYBOX, skybox);

    Shader sphereShader = Shader({"../../shader_source/basic/basic_material.vert", "../../shader_source/basic/basic_material.frag"});
    Material sphereMaterial = Material(sphereShader);
    Entity* sphere = new Entity(Mesh(sphereMaterial, Geometry::Sphere(32)));
    sphere->setPosition(0.0f, 0.0f, 0.0f);
    sphere->setTexture(ResourceLoader::getTexture(CRATE), TEXTURE_COLOR);
    

    Shader sphere2Shader = Shader({"../../shader_source/basic/basic_material.vert", "../../shader_source/basic/basic_material.frag"});
    Material sphere2Material = Material(sphere2Shader);
    Entity* sphere2 = new Entity(Mesh(sphere2Material, Geometry::Sphere(32)));
    sphere2->setPosition(-2.0f, -1.0f, 0.0f);
    sphere2->setTexture(ResourceLoader::getTexture(CRATE), TEXTURE_COLOR);

    ResourceLoader::setEntityReference("sphere", sphere);
    ResourceLoader::setEntityReference("sphere2", sphere2);
}

void Game::initalizeInputBindings() {
    GLFWwindow* window = (GLFWwindow*)IOD::glfw_window_instance;

    IOD_Profile* profile = IOD::createProfile("player");
    profile->bind(IOD_KEY_L, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    );

    profile->bind(IOD_KEY_L, IOD_InputState::RELEASED,
        []() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    );

    // Date: June 17, 2025
    // TODO(Jovanni): Investigate why glfwSetInputMode is failing in the lambda?
    profile->bind(IOD_KEY_C, IOD_InputState::PRESSED,
        []() {
            // Game::mouse_captured = !Game::mouse_captured;
            // glfwSetInputMode((GLFWwindow*)IOD::glfw_window_instance, GLFW_CURSOR, Game::mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
    );

    profile->bind(IOD_KEY_ESCAPE, IOD_InputState::PRESSED,
        [window]() {
            glfwSetWindowShouldClose(window, true);
        }
    );

    profile->bind(IOD_KEY_SPACE, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            Game::camera.process_keyboard(UP, Game::deltaTime);  
        }
    );

    profile->bind(IOD_KEY_CTRL, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            Game::camera.process_keyboard(DOWN, Game::deltaTime);  
        }
    );

    profile->bind(IOD_KEY_W, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(FORWARD, Game::deltaTime); 
        }
    );

    profile->bind(IOD_KEY_W, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(FORWARD, Game::deltaTime); 
        }
    );

    profile->bind(IOD_KEY_A, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(LEFT, Game::deltaTime); 
        }
    );

    profile->bind(IOD_KEY_S, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(BACKWARD, Game::deltaTime); 
        }
    );

    profile->bind(IOD_KEY_D, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(RIGHT, Game::deltaTime); 
        }
    );
}

// Date: June 15, 2025
// TODO(Jovanni): This is not frame independent...
float currentTime = 0;
void Game::update(GLFWwindow* window, float dt) {
    currentTime += dt;

    if (!Game::mouse_captured) {
        Game::picker.update(this->getProjectionMatrix(), Game::camera.get_view_matrix());
    }
}

void Game::render() {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    GM_Matrix4 sourceView = camera.get_view_matrix();
    GM_Matrix4 projection = this->getProjectionMatrix();

    for (const auto& key : ResourceLoader::entity_keys) {
        Entity* entity = ResourceLoader::getEntityReference(key);
        GM_Matrix4 model = entity->getTransform();
        GM_Matrix4 view = sourceView;

        if (entity->mesh.material.textures[TEXTURE_CUBEMAP] != TEXTURE_INVALID) {
            GM_Matrix4 withoutTranslation = view;
            withoutTranslation.v[0].w = 0.0f;
            withoutTranslation.v[1].w = 0.0f;
            withoutTranslation.v[2].w = 0.0f;
            view = withoutTranslation;
            model = GM_Matrix4::identity();
        } else {
            if (!Game::mouse_captured) {
                GM_Vec3 ray_origin    = Game::picker.rayOrigin;
                GM_Vec3 ray_direction = Game::picker.rayDirection;

                float ray_length = 1000.0f;
                GM_Vec3 p0 = ray_origin;
                GM_Vec3 p1 = ray_origin + (ray_direction.scale(ray_length));

                bool intersecting = GM_AABB::intersection(entity->getAABB(), p0, p1);
                if (intersecting) {
                    entity->mesh.material.color = GM_Vec4(1, 0, 0, 1);
                    entity->should_render_aabb = true;
                } else {
                    entity->mesh.material.color = GM_Vec4(1, 1, 1, 1);
                    entity->should_render_aabb = false;
                }
            } else {
                entity->mesh.material.color = GM_Vec4(1, 1, 1, 1);
                entity->should_render_aabb = false;
            }
        }

        //GM_Matrix4 mvp = projection * view * model;
        entity->draw(model, view, projection);
    }
}


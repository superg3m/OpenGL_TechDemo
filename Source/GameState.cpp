#include <GameState.hpp>

GM_Matrix4 GameState::projection;
unsigned int GameState::WINDOW_WIDTH;
unsigned int GameState::WINDOW_HEIGHT;
bool GameState::mouse_captured = true;
float GameState::timeScale = 1.0f;
Camera GameState::camera = Camera(0, 0, 5);
float GameState::deltaTime = 0.0f;
MousePicker GameState::picker = MousePicker();
float GameState::xoffset = 0.0f;
float GameState::yoffset = 0.0f;

GameState::GameState(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT) { 
    GameState::WINDOW_WIDTH = WINDOW_WIDTH;
    GameState::WINDOW_HEIGHT = WINDOW_HEIGHT;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

GLFWwindow* GameState::initalizeWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(GameState::WINDOW_WIDTH, GameState::WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        CKG_LOG_ERROR("Failed to create GLFW window\n");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    IOD_GLFW_BIND_MOUSE_MOVE_CALLBACK([](GLFWwindow *window, double mouse_x, double mouse_y) {
        local_persist bool previous_frame_mouse_was_captured = true;
        if (!GameState::mouse_captured) { 
            previous_frame_mouse_was_captured = false;
        }
        
        local_persist float last_mouse_x = mouse_x;
        local_persist float last_mouse_y = mouse_y;

        GameState::xoffset = mouse_x - last_mouse_x;
        GameState::yoffset = last_mouse_y - mouse_y;

        if (!previous_frame_mouse_was_captured && GameState::mouse_captured) {
            GameState::xoffset = 0;
            GameState::yoffset = 0;
            glfwSetCursorPos(window, last_mouse_x, last_mouse_y);
        } else {
            last_mouse_x = mouse_x;
            last_mouse_y = mouse_y;
        }  
        
        if (GameState::mouse_captured) { 
            GameState::camera.process_mouse_movements(xoffset, yoffset);
            return;
        }

        previous_frame_mouse_was_captured = true;
    });

    IOD_GLFW_BIND_KEY_CALLBACK([](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_C && action == GLFW_PRESS) {
            GameState::mouse_captured = !GameState::mouse_captured;
            glfwSetInputMode(window, GLFW_CURSOR, GameState::mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
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
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    return window;
}

GM_Matrix4 GameState::getProjectionMatrix() {
    float fov = GameState::camera.zoom;
    float aspect = (float)GameState::WINDOW_WIDTH / (float)GameState::WINDOW_HEIGHT;
    float near_plane = 0.1f;
    float far_plane = 100.0f;

    return GM_Matrix4::perspective(fov, aspect, near_plane, far_plane);
}

u64 GameState::getReferenceID() {
    local_persist u64 referenceID = 0;
    return referenceID++;
}

void GameState::initalizeResources() {
    srand(time(0));

    this->basic_shader = Shader({"../../shader_source/basic/basic.vert", "../../shader_source/basic/basic.frag"});
    this->outline_shader = Shader({"../../shader_source/outline/outline.vert", "../../shader_source/outline/outline.frag"});
    this->skybox_shader = Shader({"../../shader_source/skybox/skybox.vert", "../../shader_source/skybox/skybox.frag"});
    this->aabb_shader = Shader({"../../shader_source/aabb/aabb.vert", "../../shader_source/aabb/aabb.frag"});
    this->light_shader = Shader({"../../shader_source/light/light.vert", "../../shader_source/light/light.frag"});

    std::array<const char*, 6> cubemap_faces = {
        "../../assets/city_skybox/right.jpg",
        "../../assets/city_skybox/left.jpg",
        "../../assets/city_skybox/top.jpg",
        "../../assets/city_skybox/bottom.jpg",
        "../../assets/city_skybox/front.jpg",
        "../../assets/city_skybox/back.jpg",
    };

    TextureLoader::loadCubemapTexture(SKYBOX, cubemap_faces);
    TextureLoader::registerTexture(CRATE, "../../assets/container.jpg");
    TextureLoader::registerTexture(CRATE2, "../../assets/container2.png");
    TextureLoader::registerTexture(CRATE2_SPECULAR, "../../assets/container2_specular.png");

    Entity* skybox = new Entity(new Mesh(Geometry::Cube()));
    skybox->setTexture("uSkyboxTexture", TextureLoader::getTexture(SKYBOX));
    EntityLoader::registerSkybox(SKYBOX, skybox);

    // positions all containers
    GM_Vec3 primitivePositions[] = {
        GM_Vec3( 0.0f,  0.0f,  0.0f),
        GM_Vec3( 2.0f,  5.0f, -15.0f),
        GM_Vec3(-1.5f, -2.2f, -2.5f),
        GM_Vec3(-3.8f, -2.0f, -12.3f),
        GM_Vec3( 2.4f, -0.4f, -3.5f),
        GM_Vec3(-1.7f,  3.0f, -7.5f),
        GM_Vec3( 1.3f, -2.0f, -2.5f),
        GM_Vec3( 1.5f,  2.0f, -2.5f),
        GM_Vec3( 1.5f,  0.2f, -1.5f),
        GM_Vec3(-1.3f,  1.0f, -1.5f)
    };

    Entity* backpack = new Entity(new Model("../../assets/backpack/backpack.obj", TEXTURE_VERTICAL_FLIP));
    backpack->setPosition(GM_Vec3(-5.0f,  0.0f, 2.0f));
    // backpack->setTexture("uMaterial.diffuse", TextureLoader::getTexture(CRATE2));
    // backpack->setTexture("uMaterial.specular", TextureLoader::getTexture(CRATE2_SPECULAR));
    backpack->setEulerAngles(0, 90, 0);
    backpack->setScale(0.5f);
    EntityLoader::registerEntity("backpack", backpack);

    Entity* mouse = new Entity(new Mesh(Geometry::Cube()));
    mouse->setPosition(GM_Vec3(0, -5, 0));
    mouse->setTexture("uMaterial.diffuse", TextureLoader::getTexture(CRATE2));
    mouse->setTexture("uMaterial.specular", TextureLoader::getTexture(CRATE2_SPECULAR));
    mouse->setScale(2.0f);
    EntityLoader::registerEntity("mouse", mouse);
    
    for (int i = 0; i < ArrayCount(primitivePositions); i++) {
        Geometry geometry = (rand() % 2 == 0) ? Geometry::Cube() : Geometry::Sphere(16);
        Entity* cube = new Entity(new Mesh(geometry));
        cube->mesh->material.color = GM_Vec4(0.14f, 1.0f, 0.84f, 1);
        cube->setPosition(primitivePositions[i]);
        cube->setTexture("uMaterial.diffuse", TextureLoader::getTexture(CRATE2));
        cube->setTexture("uMaterial.specular", TextureLoader::getTexture(CRATE2_SPECULAR));
        cube->setScale(0.5f);

        EntityLoader::registerEntity("cube" + std::to_string(i), cube);
    }

    // positions of the point lights
    GM_Vec3 pointLightPositions[] = {
        GM_Vec3( 0.7f,  0.2f,  2.0f),
        GM_Vec3( 2.3f, -3.3f, -4.0f),
        GM_Vec3(-4.0f,  2.0f, -12.0f),
        GM_Vec3( 0.0f,  0.0f, -3.0f)
    };

    for (int i = 0; i < ArrayCount(pointLightPositions); i++) {
        Entity* light = new Entity(new Mesh(Geometry::Cube()));
        light->setPosition(pointLightPositions[i]);
        light->setScale(0.20f);

        EntityLoader::registerLight("light" + std::to_string(i), light);
    }
}

void GameState::initalizeInputBindings() {
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
            // GameState::mouse_captured = !GameState::mouse_captured;
            // glfwSetInputMode((GLFWwindow*)IOD::glfw_window_instance, GLFW_CURSOR, GameState::mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
    );

    profile->bind(IOD_KEY_ESCAPE, IOD_InputState::PRESSED,
        [window]() {
            glfwSetWindowShouldClose(window, true);
        }
    );

    profile->bind(IOD_KEY_SPACE, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            GameState::camera.process_keyboard(UP, GameState::deltaTime);  
        }
    );

    profile->bind(IOD_KEY_CTRL, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            GameState::camera.process_keyboard(DOWN, GameState::deltaTime);  
        }
    );

    profile->bind(IOD_KEY_F, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        [&]() {
            this->basic_shader.setBool("uUseFlashlight", true);
        }
    );

    profile->bind(IOD_KEY_F, IOD_InputState::RELEASED|IOD_InputState::UP,
        [&]() {
            this->basic_shader.setBool("uUseFlashlight", false);
        }
    );

    profile->bind(IOD_KEY_W, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(FORWARD, GameState::deltaTime); 
        }
    );

    profile->bind(IOD_KEY_W, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(FORWARD, GameState::deltaTime); 
        }
    );

    profile->bind(IOD_KEY_A, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(LEFT, GameState::deltaTime); 
        }
    );

    profile->bind(IOD_KEY_S, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(BACKWARD, GameState::deltaTime); 
        }
    );

    profile->bind(IOD_KEY_D, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        []() {
            camera.process_keyboard(RIGHT, GameState::deltaTime); 
        }
    );
}

// Date: June 15, 2025
// TODO(Jovanni): This is not frame independent...
float currentTime = 0;
Entity* entity_to_drag = nullptr;
void GameState::update(GLFWwindow* window, float dt) {
    currentTime += dt;

    int substeps = 8;
    float substep_dt = dt / (float)substeps;
    for (int step = 0; step < substeps; step++) {
        // physics
        // collisions
        // etc.
    }

    if (entity_to_drag && GameState::mouse_captured) {
        entity_to_drag->should_render_aabb = false;
        entity_to_drag = nullptr;
    } else if (!GameState::mouse_captured) {
        GameState::picker.update(this->getProjectionMatrix(), GameState::camera.get_view_matrix());
    }

    float smallest_distance = FLT_MAX;
    if (entity_to_drag && (IOD::getInputState(IOD_MOUSE_BUTTON_LEFT) == IOD_InputState::DOWN)) {
        GM_Matrix4 view = GameState::camera.get_view_matrix();
        GM_Vec4 objectViewSpace = view * GM_Vec4(entity_to_drag->position, 1.0f);
        GM_Vec3 world_space = picker.getFromObjectZ(this->getProjectionMatrix(), view, objectViewSpace.z);
        entity_to_drag->position.x = world_space.x;
        entity_to_drag->position.y = world_space.y;
    } else if (!GameState::mouse_captured) {
        if (entity_to_drag) {
            entity_to_drag->should_render_aabb = false;
        }

        entity_to_drag = nullptr;
        for (const auto& key : EntityLoader::light_keys) {
            Entity* light = EntityLoader::getLight(key);

            float ray_length = 1000.0f;
            GM_Vec3 p0 = GameState::picker.rayOrigin;
            GM_Vec3 p1 = p0 + (GameState::picker.rayDirection.scale(ray_length));
            bool intersection = GM_AABB::intersection(light->getAABB(), p0, p1);
            if (!intersection) {
                light->should_render_aabb = false;
                continue;
            }

            float current_distance = GM_Vec3::distance(light->position, GameState::camera.position);
            if (smallest_distance > current_distance) {
                if (entity_to_drag) {
                    entity_to_drag->should_render_aabb = false;
                }

                entity_to_drag = light;
                entity_to_drag->should_render_aabb = true;
                smallest_distance = current_distance;
            }
        }

        for (const auto& key : EntityLoader::entity_keys) {
            Entity* entity = EntityLoader::getEntity(key);

            float ray_length = 1000.0f;
            GM_Vec3 p0 = GameState::picker.rayOrigin;
            GM_Vec3 p1 = p0 + (GameState::picker.rayDirection.scale(ray_length));
            bool intersection = GM_AABB::intersection(entity->getAABB(), p0, p1);
            if (!intersection) {
                entity->should_render_aabb = false;
                continue;
            }

            float current_distance = GM_Vec3::distance(entity->position, GameState::camera.position);
            if (smallest_distance > current_distance) {
                if (entity_to_drag) {
                    entity_to_drag->should_render_aabb = false;
                }

                entity_to_drag = entity;
                entity_to_drag->should_render_aabb = true;
                smallest_distance = current_distance;
            }
        }
    }
}

void GameState::render() {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    GM_Matrix4 sourceView = camera.get_view_matrix();
    GM_Matrix4 projection = this->getProjectionMatrix();

    // Render Skyboxes
    glDepthFunc(GL_LEQUAL);
    for (const auto& key : EntityLoader::skybox_keys) {
        Entity* skybox = EntityLoader::getSkybox(key);

        GM_Matrix4 model = GM_Matrix4::identity();
        GM_Matrix4 withoutTranslationView = sourceView;
        withoutTranslationView.v[0].w = 0.0f;
        withoutTranslationView.v[1].w = 0.0f;
        withoutTranslationView.v[2].w = 0.0f;

        this->skybox_shader.setMat4("uMVP", projection * withoutTranslationView * model);
        skybox->draw(this->skybox_shader);
    }
    glDepthFunc(GL_LESS);

    // directional light
    this->basic_shader.use();
    this->basic_shader.setVec3("uDirLight.direction", -0.2f, -1.0f, -0.3f);
    this->basic_shader.setVec3("uDirLight.ambient", 0.05f, 0.05f, 0.05f);
    this->basic_shader.setVec3("uDirLight.diffuse", 0.4f, 0.4f, 0.4f);
    this->basic_shader.setVec3("uDirLight.specular", 0.5f, 0.5f, 0.5f);

    //  point lights
    for (int i = 0; i < EntityLoader::light_keys.size(); i++) {
        const std::string key = EntityLoader::light_keys[i];
        Entity* light = EntityLoader::getLight(key);
        GM_Matrix4 model = light->getTransform();
        GM_Matrix4 view = sourceView;

        this->basic_shader.use();
        this->basic_shader.setVec3(std::string("uPointLights[" + std::to_string(i) + "].position").c_str(), light->position);
        this->basic_shader.setVec3(std::string("uPointLights[" + std::to_string(i) + "].ambient").c_str(), 0.05f, 0.05f, 0.05f);
        this->basic_shader.setVec3(std::string("uPointLights[" + std::to_string(i) + "].diffuse").c_str(), 0.8f, 0.8f, 0.8f);
        this->basic_shader.setVec3(std::string("uPointLights[" + std::to_string(i) + "].specular").c_str(), 1.0f, 1.0f, 1.0f);
        this->basic_shader.setFloat(std::string("uPointLights[" + std::to_string(i) + "].constant").c_str(), 1.0f);
        this->basic_shader.setFloat(std::string("uPointLights[" + std::to_string(i) + "].linear").c_str(), 0.09f);
        this->basic_shader.setFloat(std::string("uPointLights[" + std::to_string(i) + "].quadratic").c_str(), 0.032f);

        this->light_shader.use();
        this->light_shader.setMat4("uModel", model);
        this->light_shader.setMat4("uView",view);
        this->light_shader.setMat4("uProjection", projection);
        this->light_shader.setVec4("uObjectColor", light->mesh->material.color);
        light->draw(this->light_shader);

        if (light->should_render_aabb) {
            model = light->getAABBTransform();
            Mesh aabb_mesh = Mesh(Geometry::AABB());
            this->aabb_shader.use();
            this->aabb_shader.setVec4("uColor", GM_Vec4(0, 1, 0, 1));
            this->aabb_shader.setMat4("uMVP", projection * view * model);
            aabb_mesh.draw(this->aabb_shader);
        }
    }

    // spotLight
    this->basic_shader.setVec3("uSpotLight.position", GameState::camera.position);
    this->basic_shader.setVec3("uSpotLight.direction", GameState::camera.front);
    this->basic_shader.setVec3("uSpotLight.ambient", 0.0f, 0.0f, 0.0f);
    this->basic_shader.setVec3("uSpotLight.diffuse", 1.0f, 1.0f, 1.0f);
    this->basic_shader.setVec3("uSpotLight.specular", 1.0f, 1.0f, 1.0f);
    this->basic_shader.setFloat("uSpotLight.constant", 1.0f);
    this->basic_shader.setFloat("uSpotLight.linear", 0.09f);
    this->basic_shader.setFloat("uSpotLight.quadratic", 0.032f);
    this->basic_shader.setFloat("uSpotLight.cutOff", cosf((float)DEGREES_TO_RAD(12.5f)));
    this->basic_shader.setFloat("uSpotLight.outerCutOff", cosf((float)DEGREES_TO_RAD(15.0f)));   

    for (const auto& key : EntityLoader::entity_keys) {
        Entity* entity = EntityLoader::getEntity(key);
        GM_Matrix4 model = entity->getTransform();
        GM_Matrix4 view = sourceView;

        this->basic_shader.use();
        this->basic_shader.setMat4("uModel", model);
        this->basic_shader.setMat4("uView", view);
        this->basic_shader.setMat4("uProjection", projection);
        this->basic_shader.setVec3("uViewPosition", GameState::camera.position);

        // material properties
        this->basic_shader.setFloat("uMaterial.shininess", 32.0f);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        entity->draw(this->basic_shader);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);

        if (entity->should_render_aabb) {
            glDisable(GL_DEPTH_TEST);
            this->outline_shader.setMat4("uModel", model);
            this->outline_shader.setMat4("uView", view);
            this->outline_shader.setMat4("uProjection", projection);
            this->outline_shader.setFloat("uOutlineScale", 0.02f);
            entity->draw(this->outline_shader, false);
            glEnable(GL_DEPTH_TEST);
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);

            model = entity->getAABBTransform();
            Mesh aabb_mesh = Mesh(Geometry::AABB());
            this->aabb_shader.use();
            this->aabb_shader.setVec4("uColor", GM_Vec4(0, 1, 0, 1));
            this->aabb_shader.setMat4("uMVP", projection * view * model);
            aabb_mesh.draw(this->aabb_shader, false);
        } else {
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);  
        }
    }
}


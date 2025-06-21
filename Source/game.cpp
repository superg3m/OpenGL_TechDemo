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

    IOD_GLFW_BIND_MOUSE_MOVE_CALLBACK([](GLFWwindow *window, double mouse_x, double mouse_y) {
        local_persist bool previous_frame_mouse_was_captured = true;
        if (!Game::mouse_captured) { 
            previous_frame_mouse_was_captured = false;
            return;
        }
        
        local_persist float last_mouse_x = mouse_x;
        local_persist float last_mouse_y = mouse_y;

        float xoffset = mouse_x - last_mouse_x;
        float yoffset = last_mouse_y - mouse_y;

        if (!previous_frame_mouse_was_captured && Game::mouse_captured) {
            xoffset = 0;
            yoffset = 0;
            glfwSetCursorPos(window, last_mouse_x, last_mouse_y);
        } else {
            last_mouse_x = mouse_x;
            last_mouse_y = mouse_y;
        }  
        
        Game::camera.process_mouse_movements(xoffset, yoffset);
        previous_frame_mouse_was_captured = true;
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

    std::map<std::string, TextureType> textures = {
        {"uColorTexture", TextureType::SAMPLER2D}
    };

    this->basic_shader = Shader(
        {"../../shader_source/basic/basic.vert", "../../shader_source/basic/basic.frag"},
        textures
    );

    textures = {
        {"uSkyboxTexture", TextureType::CUBEMAP}
    };

    this->skybox_shader = Shader(
        {"../../shader_source/skybox/skybox.vert", "../../shader_source/skybox/skybox.frag"},
        textures
    );

    this->aabb_shader = Shader(
        {"../../shader_source/aabb/aabb.vert", "../../shader_source/aabb/aabb.frag"},
        {}
    );

    this->light_shader = Shader(
        {"../../shader_source/light/light.vert", "../../shader_source/light/light.frag"},
        {}
    );

    std::array<const char*, 6> cubemap_faces = {
        "../../assets/city_skybox/right.jpg",
        "../../assets/city_skybox/left.jpg",
        "../../assets/city_skybox/top.jpg",
        "../../assets/city_skybox/bottom.jpg",
        "../../assets/city_skybox/front.jpg",
        "../../assets/city_skybox/back.jpg",
    };

    ResourceLoader::loadCubemapTexture(SKYBOX, cubemap_faces);
    ResourceLoader::loadTexture(CRATE, "../../assets/container.jpg");
    ResourceLoader::loadTexture(CRATE2, "../../assets/container2.png");
    ResourceLoader::loadTexture(CRATE2_SPECULAR, "../../assets/container2_specular.png");

    Entity* skybox = new Entity(Mesh(Geometry::Cube()));
    skybox->setTexture("uSkyboxTexture", ResourceLoader::getTexture(SKYBOX));
    ResourceLoader::setSkyboxReference(SKYBOX, skybox);

    // positions all containers
    GM_Vec3 cubePositions[] = {
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
    
    for (int i = 0; i < ArrayCount(cubePositions); i++) {
        Entity* cube = new Entity(Mesh(Geometry::Cube()));
        cube->mesh.material.color = GM_Vec4(0.14f, 1.0f, 0.84f, 1);
        cube->setPosition(cubePositions[i]);
        cube->setTexture("uMaterial.diffuse", ResourceLoader::getTexture(CRATE2));
        cube->setTexture("uMaterial.specular", ResourceLoader::getTexture(CRATE2_SPECULAR));
        cube->setScale(0.5f);

        ResourceLoader::setEntityReference("cube" + std::to_string(i), cube);
    }

    // positions of the point lights
    GM_Vec3 pointLightPositions[] = {
        GM_Vec3( 0.7f,  0.2f,  2.0f),
        GM_Vec3( 2.3f, -3.3f, -4.0f),
        GM_Vec3(-4.0f,  2.0f, -12.0f),
        GM_Vec3( 0.0f,  0.0f, -3.0f)
    };

    for (int i = 0; i < ArrayCount(pointLightPositions); i++) {
        Entity* light = new Entity(Mesh(Geometry::Cube()));
        light->setPosition(pointLightPositions[i]);
        light->setScale(0.20f);

        ResourceLoader::setLightReference("light" + std::to_string(i), light);
    }
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

    int substeps = 8;
    float substep_dt = dt / (float)substeps;
    for (int step = 0; step < substeps; step++) {
        // physics
        // collisions
        // etc.
    }

    if (!Game::mouse_captured) {
        Game::picker.update(this->getProjectionMatrix(), Game::camera.get_view_matrix());
    }

    for (const auto& key : ResourceLoader::entity_keys) {
        Entity* entity = ResourceLoader::getEntityReference(key);
        
        if (!Game::mouse_captured) {
            GM_Vec3 ray_origin    = Game::picker.rayOrigin;
            GM_Vec3 ray_direction = Game::picker.rayDirection;

            float ray_length = 1000.0f;
            GM_Vec3 p0 = ray_origin;
            GM_Vec3 p1 = ray_origin + (ray_direction.scale(ray_length));

            entity->should_render_aabb = GM_AABB::intersection(entity->getAABB(), p0, p1);
        } else {
            entity->should_render_aabb = false;
        }
    }
}

void Game::render() {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    GM_Matrix4 sourceView = camera.get_view_matrix();
    GM_Matrix4 projection = this->getProjectionMatrix();

    // Render Skyboxes
    glDepthFunc(GL_LEQUAL);
    for (const auto& key : ResourceLoader::skybox_keys) {
        Entity* skybox = ResourceLoader::getSkyboxReference(key);

        GM_Matrix4 model = GM_Matrix4::identity();
        GM_Matrix4 withoutTranslationView = sourceView;
        withoutTranslationView.v[0].w = 0.0f;
        withoutTranslationView.v[1].w = 0.0f;
        withoutTranslationView.v[2].w = 0.0f;

        this->skybox_shader.bindTexture("uSkyboxTexture", skybox->mesh.material.textures["uSkyboxTexture"]);
        this->skybox_shader.setMat4("uMVP", projection * withoutTranslationView * model);
        this->skybox_shader.unbindTextures();
        skybox->draw();
    }
    glDepthFunc(GL_LESS);

    // directional light
    this->basic_shader.use();
    this->basic_shader.setVec3("uDirLight.direction", -0.2f, -1.0f, -0.3f);
    this->basic_shader.setVec3("uDirLight.ambient", 0.05f, 0.05f, 0.05f);
    this->basic_shader.setVec3("uDirLight.diffuse", 0.4f, 0.4f, 0.4f);
    this->basic_shader.setVec3("uDirLight.specular", 0.5f, 0.5f, 0.5f);

    //  point lights
    for (int i = 0; i < ResourceLoader::light_keys.size(); i++) {
        const std::string key = ResourceLoader::light_keys[i];
        Entity* light = ResourceLoader::getLightReference(key);
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
        this->light_shader.setVec4("uObjectColor", light->mesh.material.color);
        light->draw();

        if (light->should_render_aabb) {
            model = light->getAABBTransform();
            Mesh aabb_mesh = Mesh(Geometry::AABB());
            this->aabb_shader.use();
            this->aabb_shader.setVec4("uColor", GM_Vec4(0, 1, 0, 1));
            this->aabb_shader.setMat4("uMVP", projection * view * model);
            aabb_mesh.draw();
        }
    }

    // spotLight
    this->basic_shader.setVec3("uSpotLight.position", Game::camera.position);
    this->basic_shader.setVec3("uSpotLight.direction", Game::camera.front);
    this->basic_shader.setVec3("uSpotLight.ambient", 0.0f, 0.0f, 0.0f);
    this->basic_shader.setVec3("uSpotLight.diffuse", 1.0f, 1.0f, 1.0f);
    this->basic_shader.setVec3("uSpotLight.specular", 1.0f, 1.0f, 1.0f);
    this->basic_shader.setFloat("uSpotLight.constant", 1.0f);
    this->basic_shader.setFloat("uSpotLight.linear", 0.09f);
    this->basic_shader.setFloat("uSpotLight.quadratic", 0.032f);
    this->basic_shader.setFloat("uSpotLight.cutOff", glm::cos(glm::radians(12.5f)));
    this->basic_shader.setFloat("uSpotLight.outerCutOff", glm::cos(glm::radians(15.0f)));   

    for (const auto& key : ResourceLoader::entity_keys) {
        Entity* entity = ResourceLoader::getEntityReference(key);
        GM_Matrix4 model = entity->getTransform();
        GM_Matrix4 view = sourceView;

        this->basic_shader.use();
        this->basic_shader.setMat4("uModel", model);
        this->basic_shader.setMat4("uView",view);
        this->basic_shader.setMat4("uProjection", projection);
        this->basic_shader.setVec3("uViewPosition", Game::camera.position);

        // material properties
        this->basic_shader.bindTexture("uMaterial.diffuse", entity->mesh.material.textures["uMaterial.diffuse"]);
        this->basic_shader.bindTexture("uMaterial.specular", entity->mesh.material.textures["uMaterial.specular"]);
        this->basic_shader.setFloat("uMaterial.shininess", 32.0f);
        entity->draw();
        this->basic_shader.unbindTextures();

        if (entity->should_render_aabb) {
            model = entity->getAABBTransform();
            Mesh aabb_mesh = Mesh(Geometry::AABB());
            this->aabb_shader.use();
            this->aabb_shader.setVec4("uColor", GM_Vec4(0, 1, 0, 1));
            this->aabb_shader.setMat4("uMVP", projection * view * model);
            aabb_mesh.draw();
        }
    }

}


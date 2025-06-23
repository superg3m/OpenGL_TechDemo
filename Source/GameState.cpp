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
Entity* GameState::selected_entity = nullptr;

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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CCW);
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
    this->transparent_shader = Shader({"../../shader_source/transparency/t.vert", "../../shader_source/transparency/t.frag"});
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
    TextureLoader::registerTexture(WINDOW, "../../assets/blending_transparent_window.png");

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
    backpack->setScale(0.5f);
    backpack->setEulerAngles(0, 90, 0);
    EntityLoader::registerEntity("backpack", backpack);

    Entity* window_transparent = new Entity(new Mesh(Geometry::Quad()));
    window_transparent->setPosition(GM_Vec3(-3.0f,  0.0f, 2.0f));
    window_transparent->setScale(1.0f);
    window_transparent->setEulerAngles(0, 90, 0);
    window_transparent->setTexture("uTexture", TextureLoader::getTexture(WINDOW));
    EntityLoader::registerTransparentEntity(WINDOW, window_transparent);

    Entity* window_transparent2 = new Entity(new Mesh(Geometry::Quad()));
    window_transparent2->setPosition(GM_Vec3(0.0f,  0.0f, 5.0f));
    window_transparent2->setScale(1.0f);
    window_transparent2->setTexture("uTexture", TextureLoader::getTexture(WINDOW));
    EntityLoader::registerTransparentEntity(WINDOW2, window_transparent2);
    
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

    GM_Vec4 pointLightColors[] = {
        GM_Vec4(0.7f, 0.2f,  1.0f, 1.0f),
        GM_Vec4(0.3f, 0.5f,  0.2f, 1.0f),
        GM_Vec4(1.0f, 1.0f,  1.0f, 1.0f),
        GM_Vec4(0.5f, 0.1f,  0.3f, 1.0f)
    };

    for (int i = 0; i < ArrayCount(pointLightPositions); i++) {
        Entity* light = new Entity(new Mesh(Geometry::Cube()));
        light->mesh->material.color = pointLightColors[i];
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
    
    profile->bind(IOD_KEY_UP, IOD_InputState::RELEASED,
        []() {
            if (GameState::selected_entity) {
                GameState::selected_entity->mesh->material.opacity = MIN(GameState::selected_entity->mesh->material.opacity + 0.1f, 1.0f);
            }
        }
    );

    // Date: June 17, 2025
    // TODO(Jovanni): Investigate why glfwSetInputMode is failing in the lambda?
    profile->bind(IOD_KEY_DOWN, IOD_InputState::PRESSED,
        []() {
            if (GameState::selected_entity) {
                GameState::selected_entity->mesh->material.opacity = MAX(GameState::selected_entity->mesh->material.opacity - 0.1f, 0.0f);
            }
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
void GameState::update(GLFWwindow* window, float dt) {
    currentTime += dt;

    int substeps = 8;
    float substep_dt = dt / (float)substeps;
    for (int step = 0; step < substeps; step++) {
        // physics
        // collisions
        // etc.
    }

    // sort entities based on camera and if its transparent or not!
    const auto sort_entities_by_camera = [](std::vector<std::string>& entities) {
        for (int i = 0; i < entities.size() - 1; i++) {
            Entity* entityA = EntityLoader::getEntity(entities[i]);
            float a_distance = GM_Vec3::distance(entityA->position, GameState::camera.position);
            for (int j = i + 1; j < entities.size(); j++) {
                Entity* entityB = EntityLoader::getEntity(entities[j]);

                float b_distance = GM_Vec3::distance(entityB->position, GameState::camera.position);
                if (a_distance > b_distance) {
                    std::string temp = entities[i];
                    entities[i] = entities[j];
                    entities[j] = temp;
                }
            }
        }
    };

    sort_entities_by_camera(EntityLoader::entity_keys);
    sort_entities_by_camera(EntityLoader::light_keys);
    sort_entities_by_camera(EntityLoader::transparent_keys);

    if (GameState::selected_entity && GameState::mouse_captured) {
        GameState::selected_entity->should_render_aabb = false;
        GameState::selected_entity = nullptr;
    } else if (!GameState::mouse_captured) {
        GameState::picker.update(this->getProjectionMatrix(), GameState::camera.get_view_matrix());
    }

    
    float smallest_distance = FLT_MAX;
    const auto intersection_test = [&smallest_distance](std::vector<std::string>& entities) {
        for (const auto& key : entities) {
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
                if (GameState::selected_entity) {
                    GameState::selected_entity->should_render_aabb = false;
                }

                GameState::selected_entity = entity;
                GameState::selected_entity->should_render_aabb = true;
                smallest_distance = current_distance;
                
                return;
            }
        }
    };

    if (GameState::selected_entity && (IOD::getInputState(IOD_MOUSE_BUTTON_LEFT) == IOD_InputState::DOWN)) {
        GM_Matrix4 view = GameState::camera.get_view_matrix();
        GM_Vec4 objectViewSpace = view * GM_Vec4(GameState::selected_entity->position, 1.0f);
        GM_Vec3 world_space = picker.getFromObjectZ(this->getProjectionMatrix(), view, objectViewSpace.z);
        GameState::selected_entity->position.x = world_space.x;
        GameState::selected_entity->position.y = world_space.y;
    } else if (!GameState::mouse_captured) {
        if (GameState::selected_entity) {
            GameState::selected_entity->should_render_aabb = false;
        }

        GameState::selected_entity = nullptr;
        intersection_test(EntityLoader::entity_keys);
        intersection_test(EntityLoader::light_keys);
        intersection_test(EntityLoader::transparent_keys);
    }
}

void GameState::render() {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    GM_Matrix4 sourceView = camera.get_view_matrix();
    GM_Matrix4 projection = this->getProjectionMatrix();

    // Render Skyboxes
    glDepthFunc(GL_LEQUAL);
    Mesh skybox = Mesh(Geometry::Cube());
    skybox.material.textures["uSkyboxTexture"] = TextureLoader::getTexture(SKYBOX);
    GM_Matrix4 model = GM_Matrix4::identity();
    GM_Matrix4 withoutTranslationView = sourceView;
    withoutTranslationView.v[0].w = 0.0f;
    withoutTranslationView.v[1].w = 0.0f;
    withoutTranslationView.v[2].w = 0.0f;
    this->skybox_shader.setMat4("uMVP", projection * withoutTranslationView * model);
    skybox.draw(this->skybox_shader);
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
        Entity* light = EntityLoader::getEntity(key);
        GM_Matrix4 model = light->getTransform();
        GM_Matrix4 view = sourceView;

        GM_Vec3 light_color = GM_Vec3(light->mesh->material.color);

        this->basic_shader.use();
        this->basic_shader.setVec3(std::string("uPointLights[" + std::to_string(i) + "].position").c_str(), light->position);
        this->basic_shader.setVec3(std::string("uPointLights[" + std::to_string(i) + "].ambient").c_str(), light_color.scale(0.05f));
        this->basic_shader.setVec3(std::string("uPointLights[" + std::to_string(i) + "].diffuse").c_str(), light_color.scale(0.8f));
        this->basic_shader.setVec3(std::string("uPointLights[" + std::to_string(i) + "].specular").c_str(), light_color.scale(1.0f));
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

    glEnable(GL_BLEND);
    for (int i = EntityLoader::transparent_keys.size() - 1; i >= 0; i--) {
        Entity* entity = EntityLoader::getEntity(EntityLoader::transparent_keys[i]);
        GM_Matrix4 model = entity->getTransform();
        GM_Matrix4 view = sourceView;

        this->transparent_shader.use();
        this->transparent_shader.setMat4("uModel", model);
        this->transparent_shader.setMat4("uView", view);
        this->transparent_shader.setMat4("uProjection", projection);
        this->transparent_shader.setFloat("uOpacity", entity->mesh->material.opacity);
        entity->draw(this->transparent_shader);

        if (entity->should_render_aabb) {
            model = entity->getAABBTransform();
            Mesh aabb_mesh = Mesh(Geometry::AABB());
            this->aabb_shader.use();
            this->aabb_shader.setVec4("uColor", GM_Vec4(0, 1, 0, 1));
            this->aabb_shader.setMat4("uMVP", projection * view * model);
            aabb_mesh.draw(this->aabb_shader, false);
        }
    }
    glDisable(GL_BLEND);
}


#include <GameState.hpp>

GM_Matrix4 GameState::projection;
unsigned int GameState::WINDOW_WIDTH;
unsigned int GameState::WINDOW_HEIGHT;
bool GameState::mouse_captured = true;
float GameState::timeScale = 1.0f;
Camera GameState::camera = Camera(0, 0, 5);
float GameState::deltaTime = 0.0f;
MousePicker GameState::picker = MousePicker();
Mesh* GameState::selected_mesh = nullptr;

Mesh* GameState::skybox = nullptr;
std::vector<Mesh*> GameState::meshes = {};
std::vector<Mesh*> GameState::lights = {};
std::vector<Mesh*> GameState::transparent_meshes = {};

GameState::GameState(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT) { 
    GameState::WINDOW_WIDTH = WINDOW_WIDTH;
    GameState::WINDOW_HEIGHT = WINDOW_HEIGHT;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

GM_Vec3 pointLightPositions[] = {
    GM_Vec3( 0.0f,  0.2f,  1.0f),
    GM_Vec3( 2.3f, -3.3f, -4.0f),
    GM_Vec3(-4.0f,  2.0f, -12.0f),
    GM_Vec3( 0.0f,  0.0f, -3.0f)
};

GM_Vec3 pointLightColors[] = {
    GM_Vec3(0.7f, 0.2f,  1.0f),
    GM_Vec3(0.3f, 0.5f,  0.2f),
    GM_Vec3(1.0f, 1.0f,  1.0f),
    GM_Vec3(0.5f, 0.1f,  0.3f)
};

GLFWwindow* GameState::initalizeWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

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
    glfwSetCursorPos(window, GameState::WINDOW_WIDTH / 2.0f, GameState::WINDOW_HEIGHT / 2.0f);

    IOD_GLFW_BIND_MOUSE_MOVE_CALLBACK([](GLFWwindow *window, double mouse_x, double mouse_y) {
        local_persist bool previous_frame_mouse_was_captured = true;
        if (!GameState::mouse_captured) { 
            previous_frame_mouse_was_captured = false;
        }
        
        local_persist float last_mouse_x = mouse_x;
        local_persist float last_mouse_y = mouse_y;

        float xoffset = mouse_x - last_mouse_x;
        float yoffset = last_mouse_y - mouse_y;

        if (!previous_frame_mouse_was_captured && GameState::mouse_captured) {
            xoffset = 0;
            yoffset = 0;
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
    glEnable(GL_MULTISAMPLE);
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

    this->model_shader.init();
    this->uniform_shader.init();
    this->skybox_shader.init();

    std::array<const char*, 6> cubemap_faces = {
        "../../assets/city_skybox/right.jpg",
        "../../assets/city_skybox/left.jpg",
        "../../assets/city_skybox/top.jpg",
        "../../assets/city_skybox/bottom.jpg",
        "../../assets/city_skybox/front.jpg",
        "../../assets/city_skybox/back.jpg",
    };

    TextureLoader::loadCubemapTexture(SKYBOX, cubemap_faces);
    TextureLoader::registerTexture(CRATE, "../../assets/container.jpg", true);
    TextureLoader::registerTexture(CRATE2, "../../assets/container2.png", true);
    TextureLoader::registerTexture(CRATE2_SPECULAR, "../../assets/container2_specular.png", true);
    TextureLoader::registerTexture(WINDOW, "../../assets/blending_transparent_window.png", true);

    GameState::skybox = new Mesh(Geometry::Cube());
    GameState::skybox->materials[0].textures[TEXTURE_TYPE_DIFFUSE] = TextureLoader::getTexture(SKYBOX);

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

    /*
    Mesh* rabbit = new Mesh("../../assets/rabbit/Rabbit_Low_Poly.fbx");
    rabbit->setPosition(GM_Vec3(5.0f, 0.0f, 0.0f));
    rabbit->setScale(5.0f);
    rabbit->setEulerAngles(-90, 0, 0);
    GameState::meshes.push_back(rabbit);

    Mesh* knight = new Mesh("../../assets/knight/scene.gltf");
    knight->setPosition(GM_Vec3(10.0f, 0.0f, 3.0f));
    knight->setScale(1.0f);
    knight->setEulerAngles(0, 0, 0);
    GameState::meshes.push_back(knight);
    */

    GM_Vec3 backpackBasePosition = GM_Vec3(-10.0f, 0.0f, 100.0f);   

    for (int i = 0; i < 100; i++) {
        GM_Vec3 cannonicalPosition = backpackBasePosition;
        cannonicalPosition.z -= i * 2;

        Mesh* backpack = new Mesh("../../assets/backpack/backpack.obj", TEXTURE_VERTICAL_FLIP);
        backpack->setPosition(cannonicalPosition);
        backpack->setScale(0.5f);
        backpack->setEulerAngles(0, 90, 0);
        GameState::meshes.push_back(backpack);
    }

    Mesh* window_transparent =  new Mesh(Geometry::Quad());
    window_transparent->setPosition(GM_Vec3(-3.0f,  0.0f, 2.0f));
    window_transparent->setScale(1.0f);
    window_transparent->setEulerAngles(0, 90, 0);
    window_transparent->materials[0].textures[TEXTURE_TYPE_DIFFUSE] = TextureLoader::getTexture(WINDOW);
    window_transparent->materials[0].opacity = 0.55f;
    GameState::transparent_meshes.push_back(window_transparent);

    Mesh* window_transparent2 = new Mesh(Geometry::Quad());
    window_transparent2->setPosition(GM_Vec3(0.0f,  0.0f, 5.0f));
    window_transparent2->setScale(1.0f);
    window_transparent2->setEulerAngles(0, 0, 0);
    window_transparent2->materials[0].textures[TEXTURE_TYPE_DIFFUSE] = TextureLoader::getTexture(WINDOW);
    window_transparent2->materials[0].opacity = 0.25f;
    GameState::transparent_meshes.push_back(window_transparent2);

    for (int i = 0; i < ArrayCount(primitivePositions); i++) {
        Mesh* cube = (rand() % 2 == 0) ? new Mesh(Geometry::Cube()) : new Mesh(Geometry::Sphere(16));
        cube->setScale(0.5f);
        cube->setPosition(primitivePositions[i]);

        cube->materials[0].color = GM_Vec3(0.14f, 1.0f, 0.84f);
        cube->materials[0].textures[TEXTURE_TYPE_DIFFUSE] = TextureLoader::getTexture(CRATE2);
        cube->materials[0].textures[TEXTURE_TYPE_SPECULAR] = TextureLoader::getTexture(CRATE2_SPECULAR);
        GameState::meshes.push_back(cube);
    }

    for (int i = 0; i < ArrayCount(pointLightPositions); i++) {
        Mesh* light = new Mesh(Geometry::Cube());
        light->materials[0].color = pointLightColors[i];
        light->setPosition(pointLightPositions[i]);
        light->setScale(0.20f);

        GameState::lights.push_back(light);
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
    static float current_time = 3*PI / 2.0f;
    profile->bind(IOD_KEY_T, IOD_InputState::PRESSED|IOD_InputState::DOWN,
        [&]() {
            //current_time += GameState::deltaTime;
            //this->model_shader.setFloat("uTime", current_time);
            //this->model_shader.setBool("uGeometryShader", true);
        }
    );

    profile->bind(IOD_KEY_T, IOD_InputState::RELEASED,
        [&]() {
            //current_time = 3.0f*PI / 2.0f;
            //this->model_shader.setBool("uGeometryShader", false);
        }
    );

    // Date: June 17, 2025
    // TODO(Jovanni): Investigate why glfwSetInputMode is failing in the lambda?
    profile->bind(IOD_KEY_G, IOD_InputState::PRESSED,
        [&]() {
            // GameState::mouse_captured = !GameState::mouse_captured;
            // glfwSetInputMode((GLFWwindow*)IOD::glfw_window_instance, GLFW_CURSOR, GameState::mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

            // local_persist bool useGamma = true;
            // this->basic_shader.setBool("uGamma", useGamma);
            // useGamma = !useGamma;
        }
    );
    
    profile->bind(IOD_KEY_UP, IOD_InputState::RELEASED,
        []() {
            if (GameState::selected_mesh) {
                GameState::selected_mesh->materials[0].opacity = MIN(GameState::selected_mesh->materials[0].opacity + 0.1f, 1.0f);
            }
        }
    );

    // Date: June 17, 2025
    // TODO(Jovanni): Investigate why glfwSetInputMode is failing in the lambda?
    profile->bind(IOD_KEY_DOWN, IOD_InputState::PRESSED,
        []() {
            if (GameState::selected_mesh) {
                GameState::selected_mesh->materials[0].opacity = MAX(GameState::selected_mesh->materials[0].opacity - 0.1f, 0.0f);
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
            // this->basic_shader.setBool("uUseFlashlight", true);
        }
    );

    profile->bind(IOD_KEY_F, IOD_InputState::RELEASED|IOD_InputState::UP,
        [&]() {
            // this->basic_shader.setBool("uUseFlashlight", false);
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
        (void)substep_dt;
        (void)step;
        // physics
        // collisions
        // etc.
    }

    // sort entities based on camera and if its transparent or not!
    const auto sort_entities_by_camera = [](std::vector<Mesh*>& meshes) {
        for (int i = 0; i < meshes.size(); i++) {
            Mesh* meshA = meshes[i];
            float a_distance = GM_Vec3::distance(meshA->position, GameState::camera.position);
            for (int j = i + 1; j < meshes.size(); j++) {
                Mesh* meshB = meshes[j];

                float b_distance = GM_Vec3::distance(meshB->position, GameState::camera.position);
                if (a_distance > b_distance) {
                    Mesh* temp = meshes[i];
                    meshes[i] = meshes[j];
                    meshes[j] = temp;
                }
            }
        }
    };

    sort_entities_by_camera(GameState::meshes);
    sort_entities_by_camera(GameState::lights);
    sort_entities_by_camera(GameState::transparent_meshes);

    if (GameState::selected_mesh && GameState::mouse_captured) {
        GameState::selected_mesh->should_render_aabb = false;
        GameState::selected_mesh = nullptr;
    } else if (!GameState::mouse_captured) {
        GameState::picker.update(this->getProjectionMatrix(), GameState::camera.get_view_matrix());
    }

    float smallest_distance = FLT_MAX;
    const auto intersection_test = [&smallest_distance](std::vector<Mesh*>& meshes) {
        for (Mesh* mesh : meshes) {
            float ray_length = 1000.0f;
            GM_Vec3 p0 = GameState::picker.rayOrigin;
            GM_Vec3 p1 = p0 + (GameState::picker.rayDirection.scale(ray_length));
            bool intersection = GM_AABB::intersection(mesh->getAABB(), p0, p1);
            if (!intersection) {
                mesh->should_render_aabb = false;
                continue;
            }

            float current_distance = GM_Vec3::distance(mesh->position, GameState::camera.position);
            if (smallest_distance > current_distance) {
                if (GameState::selected_mesh) {
                    GameState::selected_mesh->should_render_aabb = false;
                }

                GameState::selected_mesh = mesh;
                GameState::selected_mesh->should_render_aabb = true;
                smallest_distance = current_distance;

                return;
            }
        }
    };

    if (GameState::selected_mesh && (IOD::getInputState(IOD_MOUSE_BUTTON_LEFT) == IOD_InputState::DOWN)) {
        GM_Matrix4 view = GameState::camera.get_view_matrix();
        GM_Vec4 objectViewSpace = view * GM_Vec4(GameState::selected_mesh->position, 1.0f);
        GM_Vec3 world_space = picker.getFromObjectZ(this->getProjectionMatrix(), view, objectViewSpace.z);
        if ((GM_Vec3::dot(picker.rayDirection, GM_Vec3(-1, 0, 0)) > 0.60f) || (GM_Vec3::dot(picker.rayDirection, GM_Vec3(1, 0, 0)) > 0.60f)) {
            GameState::selected_mesh->position.z = world_space.z;
            GameState::selected_mesh->position.y = world_space.y;
        } else {
            GameState::selected_mesh->position.x = world_space.x;
            GameState::selected_mesh->position.y = world_space.y;
        }
    } else if (!GameState::mouse_captured) {
        if (GameState::selected_mesh) {
            GameState::selected_mesh->should_render_aabb = false;
        }

        GameState::selected_mesh = nullptr;
        intersection_test(GameState::meshes);
        intersection_test(GameState::lights);
        intersection_test(GameState::transparent_meshes);
    }
}

void GameState::render() {
    glClearColor(pow(0.25f, 1.0f / 2.2f), pow(0.25f, 1.0f / 2.2f), pow(0.25f, 1.0f / 2.2f), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    GM_Matrix4 sourceView = camera.get_view_matrix();
    GM_Matrix4 projection = this->getProjectionMatrix();

    // Render Skyboxes
    glDepthFunc(GL_LEQUAL);
    GM_Matrix4 skybox_model = GM_Matrix4::identity();
    GM_Matrix4 withoutTranslationView = sourceView;
    withoutTranslationView.v[0].w = 0.0f;
    withoutTranslationView.v[1].w = 0.0f;
    withoutTranslationView.v[2].w = 0.0f;

    this->skybox_shader.use();
    this->skybox_shader.setProjection(projection);
    this->skybox_shader.setView(withoutTranslationView);
    GameState::skybox->draw(skybox_shader, false);
    glDepthFunc(GL_LESS);


    //  point lights
    for (int i = 0; i < GameState::lights.size(); i++) {
        Mesh* light = GameState::lights[i];
        GM_Matrix4 light_model = light->getTransform();
        GM_Matrix4 view = sourceView;

        GM_Vec3 light_color = light->materials[0].color;

        PointLight p;
        p.position = light->position;
        p.constant = 1.0f;
        p.linear = 0.09f;
        p.quadratic = 0.032f;
        p.ambient = light_color * GM_Vec3(0.2f);
        p.diffuse = light_color * GM_Vec3(0.5f);
        p.specular = light_color * GM_Vec3(1.0f);

        this->model_shader.use();
        this->model_shader.setPointLight(p, i);

        this->uniform_shader.use();
        this->uniform_shader.setModel(light_model);
        this->uniform_shader.setView(view);
        this->uniform_shader.setProjection(projection);
        this->uniform_shader.setColor(light_color);
        light->draw(this->uniform_shader, false);

        if (light->should_render_aabb) {
            GM_Matrix4 aabb_model = light->getAABBTransform();
            Mesh aabb_mesh = Mesh(Geometry::AABB());
            this->uniform_shader.use();
            this->uniform_shader.setModel(aabb_model);
            this->uniform_shader.setView(view);
            this->uniform_shader.setProjection(projection);
            this->uniform_shader.setColor(GM_Vec3(0, 1, 0));
            aabb_mesh.draw(this->uniform_shader, false);
        }
    }


    for (Mesh* mesh : GameState::meshes) {
        GM_Matrix4 mesh_model = mesh->getTransform();
        GM_Matrix4 view = sourceView;

        this->model_shader.use();
        this->model_shader.setModel(mesh_model);
        this->model_shader.setView(view);
        this->model_shader.setProjection(projection);
        this->model_shader.setViewPosition(GameState::camera.position);

        // glStencilFunc(GL_ALWAYS, 1, 0xFF);
        // glStencilMask(0xFF);
        mesh->draw(this->model_shader);
        // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        // glStencilMask(0x00);

        if (mesh->should_render_aabb) {
            // glDisable(GL_DEPTH_TEST);
            // this->outline_shader.use();
            // this->outline_shader.setMat4("uModel", mesh_model);
            // this->outline_shader.setMat4("uView", view);
            // this->outline_shader.setMat4("uProjection", projection);
            // this->outline_shader.setFloat("uOutlineScale", 0.02f);
            // mesh->draw();
            // glEnable(GL_DEPTH_TEST);
            // glStencilMask(0xFF);
            // glStencilFunc(GL_ALWAYS, 0, 0xFF);

            GM_Matrix4 aabb_model = mesh->getAABBTransform();
            Mesh aabb_mesh = Mesh(Geometry::AABB());
            this->uniform_shader.use();
            this->uniform_shader.setModel(aabb_model);
            this->uniform_shader.setView(view);
            this->uniform_shader.setProjection(projection);
            this->uniform_shader.setColor(GM_Vec3(0, 1, 0));
            aabb_mesh.draw(this->uniform_shader, false);
        } else {
            // glStencilMask(0xFF);
            // glStencilFunc(GL_ALWAYS, 0, 0xFF);  
        }
    }

    /*
    glEnable(GL_BLEND);
    for (int i = (int)GameState::transparent_meshes.size() - 1; i >= 0; i--) {
        Mesh* mesh = GameState::transparent_meshes[i];
        GM_Matrix4 transparent_model = mesh->getTransform();
        GM_Matrix4 view = sourceView;

        this->transparent_shader.use();
        this->transparent_shader.setMat4("uModel", transparent_model);
        this->transparent_shader.setMat4("uView", view);
        this->transparent_shader.setMat4("uProjection", projection);
        this->transparent_shader.setFloat("uOpacity", mesh->materials[0].opacity);
        this->transparent_shader.setTexture("uTexture", TextureLoader::getTexture(WINDOW));
        mesh->draw(this->transparent_shader);

        if (mesh->should_render_aabb) {
            GM_Matrix4 aabb_model = mesh->getAABBTransform();
            Mesh aabb_mesh = Mesh(Geometry::AABB());
            this->uniform_shader.use();
            this->uniform_shader.setModel(aabb_model);
            this->uniform_shader.setView(view);
            this->uniform_shader.setProjection(projection);
            this->uniform_shader.setColor(GM_Vec3(0, 1, 0));
            aabb_mesh.draw();
        }
    }
    glDisable(GL_BLEND);
    */
}
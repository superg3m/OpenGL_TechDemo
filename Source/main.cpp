#include <main.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(0.0f, 0.0f, 3.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

std::vector<float> vertices = {
    // Front face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // Top-right
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // Top-right
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // Top-left
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // Bottom-left

    // Back face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // Top-right
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // Top-right
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, // Top-left
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // Bottom-left

    // Left face
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // Top-right
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // Top-left
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // Bottom-left
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // Bottom-left
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // Bottom-right
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // Top-right

    // Right face
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // Top-right
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // Top-left
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // Bottom-left
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // Top-right

    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // Top-left
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // Top-right
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // Bottom-right
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // Bottom-right
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // Bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // Top-left

    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // Top-left
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // Top-right
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // Bottom-right
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, // Bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f  // Top-left
};

std::vector<float> quad_vertices = {
    // Front face of the quad
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // Top-right
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // Top-right
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // Top-left
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f  // Bottom-left
};

std::vector<float> line_vertices = {
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f
};

bool fire_tracer = false;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader cubeShader({"../../shader_source/model.vert", "../../shader_source/model.frag"});
    cubeShader.addTexture("../../assets/container.jpg", "texture1", TEXTURE_VERTICAL_FLIP);
    Mesh cubeMesh(Material(&cubeShader), Geometry({3, 3, 2}, vertices));
    GM_Vec3 cube_position = GM_Vec3Lit(-0.5, -0.5, -0.5);
    GM_RectangleReference3D cube_aabb = gm_rectangle_reference3d_create(&cube_position, 1, 1, 1);

    Shader cube2Shader({"../../shader_source/model.vert", "../../shader_source/model.frag"});
    cube2Shader.addTexture("../../assets/container.jpg", "texture1", TEXTURE_VERTICAL_FLIP);
    Mesh explosionCubeMesh(Material(&cube2Shader), Geometry({3, 3, 2}, vertices));

    Shader billboardShader({"../../shader_source/billboard.vert", "../../shader_source/billboard.frag"});
    billboardShader.addTexture("../../assets/slime_monster.png", "texture1", TEXTURE_VERTICAL_FLIP);
    Mesh quadMesh(Material(&billboardShader), Geometry({3, 3, 2}, quad_vertices));
    GM_Vec3 billboard_position = GM_Vec3Lit(-1, -2, -3);
    GM_RectangleReference3D quad_aabb = gm_rectangle_reference3d_create(&billboard_position, 1, 1, 1);

    Shader explosionShader({"../../shader_source/billboard.vert", "../../shader_source/billboard.frag"});
    explosionShader.addTexture("../../assets/explosion.png", "texture1", TEXTURE_VERTICAL_FLIP);
    Mesh explosionMesh(Material(&explosionShader), Geometry({3, 3, 2}, quad_vertices));

    GM_Vec3 lightPos = gm_vec3_create(-1.2f, 1.0f, 2.0f);
    Shader lightCubeShader({"../../shader_source/light_cube.vert", "../../shader_source/light_cube.frag"});
    Mesh lightCubeMesh(Material(&lightCubeShader), Geometry({3, 3, 2}, vertices));

    Shader lineShader({"../../shader_source/billboard.vert", "../../shader_source/billboard.frag"});
    Mesh lineMesh(Material(&lineShader), Geometry({3}, line_vertices, {}, GL_DYNAMIC_DRAW), gm_mat4_identity(), GL_LINES);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    float explosion_scale_goal = 0.4f;
    float explosion_scale_current = explosion_scale_goal;

    glfwSwapInterval(1);

    GM_Vec3 intersection;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        GM_Matrix4 view = camera.get_view_matrix();
        GM_Matrix4 projection = gm_mat4_perspective(camera.zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        cubeMesh.transform = gm_mat4_identity();
        cubeMesh.transform = gm_mat4_scale_xyz(cubeMesh.transform, 0.45f, 0.45f, 0.45f);
        cubeMesh.material.shader->setMat4("view", view);
        cubeMesh.material.shader->setMat4("projection", projection);
        cubeMesh.material.shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        cubeMesh.material.shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        cubeMesh.material.shader->setVec3("lightPos", lightPos);
        cubeMesh.material.shader->setVec3("viewPos", camera.position);
        cubeMesh.draw();

        
        // billboard sprite
        GM_Matrix4 billboard_model = gm_mat4_transpose(view);
        billboard_model.v[0].w = billboard_position.x;
        billboard_model.v[1].w = billboard_position.y;
        billboard_model.v[2].w = billboard_position.z;
        quadMesh.transform = billboard_model;
        quadMesh.material.shader->setMat4("view", view);
        quadMesh.material.shader->setMat4("projection", projection);
        quadMesh.draw();


        // line sprite
        if (fire_tracer) {
            GM_Vec3 p0 = gm_vec3_add(camera.position, GM_Vec3Lit(0.25f, -0.25f, 0));
            GM_Vec3 p1 = gm_vec3_add(p0, gm_vec3_scale(camera.front, 10.0f));
            line_vertices[0] = p0.x;
            line_vertices[1] = p0.y;
            line_vertices[2] = p0.z;

            line_vertices[3] = p1.x;
            line_vertices[4] = p1.y;
            line_vertices[5] = p1.z;

            lineMesh.setVertices(line_vertices);

            lineMesh.transform = gm_mat4_identity();
            lineMesh.material.shader->setMat4("view", view);
            lineMesh.material.shader->setMat4("projection", projection);
            lineMesh.draw();


            if (gm_intersection3d_line_aabb(p0, p1, cube_aabb, &intersection, NULLPTR)) {
                line_vertices[3] = intersection.x;
                line_vertices[4] = intersection.y;
                line_vertices[5] = intersection.z;
                explosion_scale_current = 0.0f;
            } else {
                line_vertices[3] = p1.x;
                line_vertices[4] = p1.y;
                line_vertices[5] = p1.z;
            }

            fire_tracer = false;
        }

        if (explosion_scale_current != explosion_scale_goal) {
            /*
            GM_Matrix4 explosion_model = gm_mat4_transpose(view);
            explosion_model.v[0].w = intersection.x;
            explosion_model.v[1].w = intersection.y;
            explosion_model.v[2].w = intersection.z;

            explosion_scale_current = gm_move_toward(explosion_scale_current, explosion_scale_goal, 0.02f);
            explosion_model.v[0].x *= explosion_scale_current;
            explosion_model.v[1].y *= explosion_scale_current;
            explosion_model.v[2].z *= explosion_scale_current;
            
            explosionMesh.transform = explosion_model;

            explosionMesh.material.shader->setMat4("view", view);
            explosionMesh.material.shader->setMat4("projection", projection);
            explosionMesh.draw();
            */

            cubeMesh.transform = gm_mat4_identity();
            explosion_scale_current = gm_move_toward(explosion_scale_current, explosion_scale_goal, 0.02f);
            cubeMesh.transform = gm_mat4_scale_xyz(cubeMesh.transform, explosion_scale_goal, explosion_scale_goal, explosion_scale_goal);
            cubeMesh.transform = gm_mat4_translate(cubeMesh.transform, intersection);

            cubeMesh.material.shader->setMat4("view", view);
            cubeMesh.material.shader->setMat4("projection", projection);
            cubeMesh.material.shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            cubeMesh.material.shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            cubeMesh.material.shader->setVec3("lightPos", lightPos);
            cubeMesh.material.shader->setVec3("viewPos", camera.position);
            cubeMesh.draw();
        }


        lightCubeMesh.transform = gm_mat4_identity();
        lightCubeMesh.transform = gm_mat4_scale_xyz(lightCubeMesh.transform, 0.5f, 0.5f, 0.5f);
        lightCubeMesh.transform = gm_mat4_translate(lightCubeMesh.transform, lightPos);
        lightCubeMesh.material.shader->setMat4("view", view);
        lightCubeMesh.material.shader->setMat4("projection", projection);
        lightCubeMesh.draw();
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        fire_tracer = true;
    } else {
        fire_tracer = false;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.process_keyboard(UP, deltaTime);    
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.process_keyboard(DOWN, deltaTime);        
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.process_keyboard(FORWARD, deltaTime);        
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.process_keyboard(BACKWARD, deltaTime);        
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.process_keyboard(LEFT, deltaTime);        
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.process_keyboard(RIGHT, deltaTime);        
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = (float)xposIn;
    float ypos = (float)yposIn;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.process_mouse_movements(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.process_mouse_scroll((float)yoffset);
}
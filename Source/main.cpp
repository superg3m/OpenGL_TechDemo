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

// Found at: https://www.khronos.org/opengl/wiki/GluLookAt_code
GM_Matrix4 gm_mat4_look_at_model(GM_Vec3 position, GM_Vec3 target, GM_Vec3 world_up) {
    GM_Vec3 forward = gm_vec3_normalize(gm_vec3_sub(target, position));
    GM_Vec3 right   = gm_vec3_normalize(gm_vec3_cross(world_up, forward));
    GM_Vec3 up      = gm_vec3_cross(forward, right);

    float dot_right   = -gm_vec3_dot(right, position);
    float dot_up      = -gm_vec3_dot(up, position);
    float dot_forward = -gm_vec3_dot(forward, position);

    GM_Matrix4 rotation = {
        .data = {
            right.x,   right.y,   right.z,   0,
            up.x,      up.y,      up.z,      0,
            forward.x, forward.y, forward.z, 0,
            0.0f,      0.0f,      0.0f,      1.0f
        }
    };
    
    GM_Matrix4 translation = gm_mat4_translate_xyz(gm_mat4_identity(), -position.x, -position.y, -position.z);

    return gm_mat4_mult(rotation, translation);
}

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
    Mesh cubeMesh(GL_TRIANGLES, Material(&cubeShader), Geometry({3, 3, 2}, vertices));

    
    Shader billboardShader({"../../shader_source/billboard.vert", "../../shader_source/billboard.frag"});
    billboardShader.addTexture("../../assets/slime_monster.png", "texture1", TEXTURE_VERTICAL_FLIP);
    Mesh quadMesh(GL_TRIANGLES, Material(&billboardShader), Geometry({3, 3, 2}, quad_vertices));

    GM_Vec3 lightPos = gm_vec3_create(-1.2f, 1.0f, 2.0f);
    Shader lightCubeShader({"../../shader_source/light_cube.vert", "../../shader_source/light_cube.frag"});
    Mesh lightCubeMesh(GL_TRIANGLES, Material(&lightCubeShader), Geometry({3, 3, 2}, vertices));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    GM_Vec3 billboard_position = GM_Vec3Lit(-1, -2, -3);

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
        cubeMesh.transform = gm_mat4_rotate_xyz(cubeMesh.transform, RAD_TO_DEGREES((float)glfwGetTime()), 0.5f, 1.0f, 0.0f);
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
        quadMesh.material.shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        quadMesh.material.shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        quadMesh.material.shader->setVec3("lightPos", lightPos);
        quadMesh.material.shader->setVec3("viewPos", camera.position);
        quadMesh.draw();


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
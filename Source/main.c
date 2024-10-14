#include <ckit.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <mesh.h>
#include <stb_image.h>

void window_resize_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
void window_refresh_callback(GLFWwindow* window);

// settings
const u32 SCR_WIDTH = 800;
const u32 SCR_HEIGHT = 600;

Shader rect_shader;
unsigned int VBO, VAO, EBO;
Mesh rect_mesh;

int main() {
    ckit_init();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef PLATFORM_APPLE
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        LOG_FATAL("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, window_resize_callback);
    glfwSetWindowRefreshCallback(window, window_refresh_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_FATAL("Failed to initialize GLAD\n");
        return -1;
    }

    // assumes the extension is supported .frag, .vert, .compute, ect...
    const char* rect_shader_paths[] = {
        "../shader_source/basic.frag",
        "../shader_source/basic.vert"
    };

    rect_shader = shader_create(rect_shader_paths, ArrayCount(rect_shader_paths));

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    u32 vertex_components[] = {3, 3, 2};
    CKIT_Vector3 rect_postion = {0.0, 0.0, 0.0};
    VertexBuffer vertex_buffer = vertex_buffer_create(vertices, ArrayCount(vertices), vertex_components, ArrayCount(vertex_components)); 
    rect_mesh = mesh_create(rect_postion, &rect_shader, vertex_buffer, indices, ArrayCount(indices), GL_STATIC_DRAW);
    shader_add_texture(&rect_shader, "../assets/container.jpg", "texture1", TEXTURE_VERTICAL_FLIP);
    shader_add_texture(&rect_shader, "../assets/awesomeface.png", "texture2", TEXTURE_VERTICAL_FLIP);


    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mesh_draw(&rect_mesh);
        // game_update();
        // game_render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    shader_free(&rect_shader);

    glfwTerminate();
    ckit_cleanup();
    return 0;
}

void process_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}


void window_resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void window_refresh_callback(GLFWwindow* window) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mesh_draw(&rect_mesh);

    glfwSwapBuffers(window);
}
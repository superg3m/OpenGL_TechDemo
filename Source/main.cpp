#include <ckit.h>

// #include <game.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <shader.h>
#include <mesh.h>
#include <vertex_data.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 camera(float Translate, glm::vec2 Rotate) {
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
} 

void window_resize_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
void window_refresh_callback(GLFWwindow* window);

// settings
const u32 SCR_WIDTH = 800;
const u32 SCR_HEIGHT = 600;

float* vertices_vec = NULLPTR;
u32* indices_vec = NULLPTR;

float* vertices_vec2 = NULLPTR;
float* vertices_vec3 = NULLPTR;

RenderGroup primative_group;

Mesh rect_mesh;
Mesh rect_mesh2;

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

    Shader primative_shader = shader_create(rect_shader_paths, ArrayCount(rect_shader_paths));
    ckit_vector_push(primative_shader.attributes, 3);
    ckit_vector_push(primative_shader.attributes, 3);
    ckit_vector_push(primative_shader.attributes, 2);

    Shader primative_shader2 = shader_create(rect_shader_paths, ArrayCount(rect_shader_paths));
    ckit_vector_push(primative_shader2.attributes, 3);
    ckit_vector_push(primative_shader2.attributes, 3);
    ckit_vector_push(primative_shader2.attributes, 2);

    // ------------------------------------------------------ VERTEX STUFF ------------------------------------------------------------

    vertices_vec = ckit_vector_reserve(ArrayCount(vertices), float);
    ckit_memory_copy(vertices, vertices_vec, ArrayCount(vertices) * sizeof(float), ArrayCount(vertices) * sizeof(float));
    ckit_vector_base(vertices_vec)->count = ArrayCount(vertices);

    indices_vec = ckit_vector_reserve(ArrayCount(indices), u32);
    ckit_memory_copy(indices, indices_vec, ArrayCount(indices) * sizeof(u32), ArrayCount(indices) * sizeof(u32));
    ckit_vector_base(indices_vec)->count = ArrayCount(indices);

    vertices_vec2 = ckit_vector_reserve(ArrayCount(vertices2), float);
    ckit_memory_copy(vertices2, vertices_vec2, ArrayCount(vertices2) * sizeof(float), ArrayCount(vertices2) * sizeof(float));
    ckit_vector_base(vertices_vec2)->count = ArrayCount(vertices2);

    vertices_vec3 = ckit_vector_reserve(ArrayCount(vertices3), float);
    ckit_memory_copy(vertices3, vertices_vec3, ArrayCount(vertices3) * sizeof(float), ArrayCount(vertices3) * sizeof(float));
    ckit_vector_base(vertices_vec3)->count = ArrayCount(vertices3);

    // ------------------------------------------------------------------------------------------------------------------
 
    primative_group = render_group_create(GL_TRIANGLES);
    glm::vec rect_postion = glm::vec3(0.0, 0.0, 0.0);

    // Date: October 15, 2024
    // TODO(Jovanni): You can get rid of specifying textures[0] u can just do that internally
    rect_mesh = mesh_create(&primative_shader, vertices_vec, indices_vec, GL_STATIC_DRAW);
    shader_add_texture(&primative_shader, "../assets/container.jpg", "textures[0]", TEXTURE_PIXEL_PERFECT|TEXTURE_VERTICAL_FLIP);
    shader_add_texture(&primative_shader, "../assets/awesomeface.png", "textures[1]", TEXTURE_VERTICAL_FLIP);
    mesh_set_position(&rect_mesh, glm::vec3(0.5f, -0.5f, 0.0f));

    rect_mesh2 = mesh_create(&primative_shader2, vertices_vec2, indices_vec, GL_DYNAMIC_DRAW);
    shader_add_texture(&primative_shader2, "../assets/container.jpg", "textures[0]", TEXTURE_VERTICAL_FLIP);

    render_group_add(&primative_group, &rect_mesh);
    render_group_add(&primative_group, &rect_mesh2);

    glm::mat4 camera_matrix = camera(1.0, glm::vec2(3.0));

    // game_init();

    while (!glfwWindowShouldClose(window)) {
        double start_time = glfwGetTime();
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const float SPEED = 2.0f;
        mesh_set_rotation(&rect_mesh, glfwGetTime() * SPEED, glm::vec3(0.0f, 0.0f, 1.0f));
        mesh_set_scale(&rect_mesh, glm::vec3(sinf(glfwGetTime())));

        render_group_draw(&primative_group);
        // void game_update_and_render(double delta)

        glfwSwapBuffers(window);
        glfwPollEvents();

        double end_time = glfwGetTime();
        double delta_time_seconds = end_time - start_time;
        // LOG_DEBUG("%llfms\n", delta_time_seconds * 1000);
        // LOG_ERROR("FPS: %lld\n", (u64)(1.0 / delta_time_seconds));
    }

    render_group_free(&primative_group);
    shader_free(&primative_shader);
    shader_free(&primative_shader2);
    ckit_vector_free(vertices_vec);
    ckit_vector_free(indices_vec);
    ckit_vector_free(vertices_vec2);
    ckit_vector_free(vertices_vec3);

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

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        mesh_set_vertices(&rect_mesh2, vertices_vec3);
    } else {
        mesh_set_vertices(&rect_mesh2, vertices_vec2);
    }
}


void window_resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void window_refresh_callback(GLFWwindow* window) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const float SPEED = 2.0f;
    mesh_set_rotation(&rect_mesh, glfwGetTime() * SPEED, glm::vec3(0.0f, 0.0f, 1.0f)); // This is probably getting messed up because glfwGetTime()
    mesh_set_scale(&rect_mesh, glm::vec3(sinf(glfwGetTime()))); // This is probably getting messed up because glfwGetTime()

    render_group_draw(&primative_group);

    glfwSwapBuffers(window);
}
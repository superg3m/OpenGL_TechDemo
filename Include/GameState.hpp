#pragma once

#include <ctime>

#include <ckg.h>
#include <gm.hpp>
#include <GLFW_IOD.hpp>

#include <EntityLoader.hpp>
#include <TextureLoader.hpp>

#include <Mesh.hpp>
#include <MousePicker.hpp>
#include <Camera.hpp>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#define SKYBOX "Skybox"
#define CRATE "Crate"
#define CRATE2 "Crate2"
#define CRATE2_SPECULAR "Crate2_Specular"

#define WINDOW "Window"
#define WINDOW2 "Window2"

struct DrawElementsCommand {
    unsigned int  count;
    unsigned int  instanceCount;
    unsigned int  firstIndex;
    int  baseVertex;
    unsigned int  baseInstance;
};
 
struct GameState {
    static bool mouse_captured;
    static float timeScale;
    static GM_Matrix4 projection;
    static unsigned int WINDOW_WIDTH;
    static unsigned int WINDOW_HEIGHT;
    static Camera camera;
    static float deltaTime;
    static MousePicker picker;
    static float xoffset;
    static float yoffset;
    static Entity* selected_entity;
    static std::vector<DrawElementsCommand> command_buffer;

    Shader basic_shader;
    Shader outline_shader;
    Shader skybox_shader;
    Shader aabb_shader;
    Shader light_shader;
    Shader particle_shader;
    Shader pbr_shader;
    Shader transparent_shader;

    GameState(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

    GLFWwindow* initalizeWindow();
    void initalizeResources();
    void initalizeInputBindings();
    GM_Matrix4 getProjectionMatrix();
    void update(GLFWwindow* window, float dt);
    void render();

    static u64 getReferenceID();
private:

};
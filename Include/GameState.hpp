#pragma once

#include <ctime>

#include <ckg.h>
#include <gm.hpp>
#include <GLFW_IOD.hpp>

#include <TextureLoader.hpp>

#include <Mesh.hpp>
#include <MousePicker.hpp>
#include <Camera.hpp>

#include <ShaderModel.hpp>
#include <ShaderSkybox.hpp>
#include <ShaderUniformColor.hpp>
#include <ShaderTransparency.hpp>
#include <ShaderStencilOutline.hpp>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#define SKYBOX "Skybox"
#define CRATE "Crate"
#define CRATE2 "Crate2"
#define CRATE2_SPECULAR "Crate2_Specular"

#define WINDOW "Window"

/*
struct DrawElementsCommand {
    unsigned int  count;
    unsigned int  instanceCount;
    unsigned int  firstIndex;
    int  baseVertex;
    unsigned int  baseInstance;
};
*/

 struct GameState {
    static bool mouse_captured;
    static float timeScale;
    static GM_Matrix4 projection;
    static unsigned int WINDOW_WIDTH;
    static unsigned int WINDOW_HEIGHT;
    static Camera camera;
    static float deltaTime;
    static MousePicker picker;

    static Mesh* skybox;
    static Mesh* selected_mesh;
    static std::vector<Mesh*> meshes;
    static std::vector<Mesh*> lights;
    static std::vector<Mesh*> transparent_meshes;

    static bool use_flashlight;

    ShaderModel model_shader;
    ShaderSkybox skybox_shader;
    ShaderUniformColor uniform_shader; // aabb and lights
    ShaderStencilOutline outline_shader;
    ShaderTransparency transparent_shader;

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
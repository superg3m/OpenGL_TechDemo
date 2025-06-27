#define MAX_SPOT_LIGHT_COUNT 4
#define MAX_POINT_LIGHT_COUNT 4

#define MAX_MESH_COUNT 16

struct Scene {
    DirectionLight sun;
    PointLight point_lights[MAX_POINT_LIGHT_COUNT];
    SpotLight spot_lights[MAX_SPOT_LIGHT_COUNT];

    Mesh* meshes[MAX_MESH_COUNT]

    void pushMesh(Mesh* mesh);
    void pushPointLight(PointLight point_light);
    void pushSpotLight(SpotLight spot_light);
    void setDirectionalLight(DirectionalLight directional_light);

    draw();
};

scene_init() {
    // push a bunch of things to the scene
}
scene_update(Engine* engine, GameState* state)
scene_render(Engine* engine, GameState* state)
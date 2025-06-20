/*
struct VertexArrayObject {
    unsigned int vao;

    int getStride() const;
    void bind() const;
};

// Date: June 19, 2025
// NOTE(Jovanni): VAO has the attributes to interpret the VBO which has the float data
std::Vector<VertexAttribute> attributes = {
    VertexAttribute{UniformType::VEC3, "aPosition"},
    VertexAttribute{UniformType::VEC3, "aNormal"},
    VertexAttribute{UniformType::VEC2, "aUV"}
    VertexAttribute{UniformType::VEC2, "aUV"}
    VertexAttribute{UniformType::VEC2, "aUV"}
    VertexAttribute{UniformType::VEC2, "aUV"}
}

std::vector<std::string> shaderUniforms = {
    "uColor",
    "uTexture"
    "uTextureMask"
    "uCubeMap"
}

Shader shader = Shader(paths, attributes, uniforms);
Mesh mesh = Mesh(vertices, indicices)

so you have predefiend shaders for specfic things:
BasicLitShader: For objects with diffuse, specular, and ambient lighting.
UnlitShader: For objects that just display a texture, like UI elements or particles.
PBRShader: For physically based rendering.
SkyboxShader: Specialized for rendering skyboxes.
PostProcessShader: For full-screen effects.

Then you just have:
BasicShader.setVec4("uColor", entity->color);
BasicShader.setVec4("u", entity->textures);
BasicShader.setMat4("uMVP", projection * view * model);
BasicShader.setMat4("uMVP", projection * view * model);

void render(Scene& scene, const Camera& camera, const std::vector<Light>& lights) {
        // --- PRE-PASS: Shadow Map Generation ---
        // Render from each light's perspective to a depth texture
        for (const auto& light : lights) {
            if (light.castsShadows) {
                // Bind shadow map FBO
                depthShader->use(); // Activate the specialized depth shader
                depthShader->setMat4("lightSpaceMatrix", light.getLightSpaceMatrix());

                // Iterate through entities with MeshRenderComponent
                // Draw only their depth, no fancy materials needed
                for (auto& entity : scene.getEntitiesWith<TransformComponent, MeshRenderComponent>()) {
                    auto& transform = entity.get<TransformComponent>();
                    auto& meshRender = entity.get<MeshRenderComponent>();
                    if (meshRender.castsShadow) {
                        depthShader->setMat4("model", transform.getModelMatrix());
                        glBindVertexArray(meshRender.mesh->vao);
                        glDrawElements(...);
                    }
                }
                // Unbind shadow map FBO
            }
        }

        // --- MAIN PASS: 3D Scene Rendering ---
        // Sort renderable objects by shader, then material, then mesh for optimal batching
        std::vector<RenderableObjectData> opaqueObjects; // Contains Transform, Mesh, Material pointers
        std::vector<RenderableObjectData> transparentObjects; // Sorted back-to-front

        for (auto& entity : scene.getEntitiesWith<TransformComponent, MeshRenderComponent>()) {
            // Populate opaque/transparent lists
        }
        // ... also gather particle emitters and UI elements

        // Render opaque objects
        Shader* currentShader = nullptr;
        for (const auto& objData : opaqueObjects) {
            // If the object's material uses a different shader, switch!
            if (objData.material->shaderProgram != currentShader) {
                currentShader = objData.material->shaderProgram;
                currentShader->use(); // <-- SHADER SWITCH HAPPENS HERE!

                // Set global uniforms that this shader needs (view, projection, light info)
                // The shader itself dictates what uniforms it *has*.
                // You could query shader uniform locations once on load.
                if (currentShader == pbrShader) {
                    pbrShader->setMat4("view", camera.getViewMatrix());
                    pbrShader->setMat4("projection", camera.getProjectionMatrix());
                    pbrShader->setVec3("cameraPos", camera.getPosition());
                    // ... set light uniforms for PBR shader
                    // ... bind shadow map texture for PBR shader
                }
                // ... handle global uniforms for other specialized shaders
            }

            // Set per-object uniforms
            currentShader->setMat4("model", objData.transform->getModelMatrix());

            // Apply material-specific uniforms (this is where the Material's data gets sent)
            objData.material->apply(currentShader); // Material tells its uniforms to the active shader

            // Draw the mesh
            glBindVertexArray(objData.mesh->vao);
            glDrawElements(...);
        }

        // --- PARTICLE PASS ---
        particleShader->use(); // <-- SHADER SWITCH
        particleShader->setMat4("view", camera.getViewMatrix());
        particleShader->setMat4("projection", camera.getProjectionMatrix());
        // ... set other global particle shader uniforms

        for (auto& entity : scene.getEntitiesWith<TransformComponent, ParticleEmitterComponent>()) {
            auto& transform = entity.get<TransformComponent>();
            auto& emitter = entity.get<ParticleEmitterComponent>();
            // Update particles (CPU-side logic)
            // Bind emitter's VAO
            emitter.material->apply(particleShader); // Apply particle-specific texture/color uniforms
            glDrawArraysInstanced(...); // Or similar draw call for particles
        }


        // --- UI PASS ---
        // Often uses an orthographic projection and disables depth testing
        unlitUIShader->use(); // <-- SHADER SWITCH
        unlitUIShader->setMat4("projection", glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight));
        unlitUIShader->setMat4("view", glm::mat4(1.0f)); // Identity view for UI

        for (auto& entity : scene.getEntitiesWith<TransformComponent, SpriteRenderComponent>()) {
            auto& transform = entity.get<TransformComponent>();
            auto& sprite = entity.get<SpriteRenderComponent>();

            unlitUIShader->setMat4("model", transform.getModelMatrix()); // Or just 2D position/scale
            sprite.material->apply(unlitUIShader); // Apply UI-specific texture/color uniforms
            glBindVertexArray(sprite.mesh->vao); // Assumes UI elements are rendered as quads
            glDrawElements(...);
        }

        // ... continue with other passes (post-processing, transparent objects, etc.)
    }
*/
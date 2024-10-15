The goal of this project is to build a non-trivial scene in opengl going through all of LearnOpenGL's tutorial
Then I want to replace GLFW and glad with my own CKIT_GFW (CKIT_Graphics FrameWork)

Probably will want to do batching to reduce the number of VAOs I need and 


// There is instancing which typically is rendering a bunch of the same mesh but wanting to have different transforms. This is useful because with the same mesh you can
offload pretty much all of the commpute to the gpu with instanceID which is perferable. This is all one draw call and typically you would use a specially designed shader for instancing. Something like particles is a really good candidate for instancing.

// Then there is batching which is typically rendering multiple different meshes with one draw call the only thing you have to be careful of is if the layout/Number of VertexAttributeComponents is the same. IF they are then you can just store the offset and transforms and with one draw call you can render multiple meshes at once. Keep in mind that each mesh still needs a unique shader instance I believe. But they can use the same or different shader source shouldn't matter much.

// I haven't done the batching yet, but I have a good idea on how to do it and I think i'm pretty close.

void glMultiDrawArrays(GLenum mode, const GLint * first, const GLsizei * count, GLsizei drawcount);

const char* rectShaderPaths[] = {
    "../shader_source/basic.frag",
    "../shader_source/basic.vert"
};

Shader primative_shader(rectShaderPaths);
RenderGroup primativeGroup = render_group_create(GL_TRIANGLES, GL_STATIC_DRAW);
Mesh rect({0, 0, 0}, &rectShader, getSquareGeometry(side_length));
Mesh circle(&rect_shader, getCircleGeometry(radius));
primativeGroup->add({rect, circle}); // ensures that that every single shader in the mesh has the same attributes


// Vector<Mesh> getModelGeometry(path)
RenderGroup modelGroup(GL_TRIANGLES, GL_STATIC_DRAW);
Model backpack_model({1, 10, 5}, &model_shader, getModelGeometry(backpack_path));
Model skull_model({1, 10, 5}, &model_shader, getModelGeometry(skull_path));
modelGroup->add({backpack_model, skull_model});

primativeGroup->draw();
modelGroup->draw();
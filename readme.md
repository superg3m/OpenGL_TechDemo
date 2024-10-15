The goal of this project is to build a non-trivial scene in opengl going through all of LearnOpenGL's tutorial
Then I want to replace GLFW and glad with my own CKIT_GFW (CKIT_Graphics FrameWork)

Probably will want to do batching to reduce the number of VAOs I need and 


// There is instancing which typically is rendering a bunch of the same mesh but wanting to have different transforms. This is useful because with the same mesh you can
offload pretty much all of the commpute to the gpu with instanceID which is perferable. This is all one draw call and typically you would use a specially designed shader for instancing. Something like particles is a really good candidate for instancing.

// Then there is batching which is typically rendering multiple different meshes with one draw call the only thing you have to be careful of is if the layout/Number of VertexAttributeComponents is the same. IF they are then you can just store the offset and transforms and with one draw call you can render multiple meshes at once. Keep in mind that each mesh still needs a unique shader instance I believe. But they can use the same or different shader source shouldn't matter much.

// I haven't done the batching yet, but I have a good idea on how to do it and I think i'm pretty close.

void glMultiDrawArrays(GLenum mode, const GLint * first, const GLsizei * count, GLsizei drawcount);
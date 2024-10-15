#pragma once

float vertices[] = {
    // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

float vertices2[] = {
    // positions          // colors           // texture coords
    +0.25f, +0.25f, +0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    +0.25f, -0.25f, +0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.25f, -0.25f, +0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // bottom left
    -0.25f, +0.25f, +0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left 
};

// supppsed to me messed up
float vertices3[] = {
    // positions          // colors           // texture coords
    +0.15f, +0.5f, +0.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top right
    +0.15f, -0.5f, +0.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
    -0.15f, -0.15f, +0.0f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.15f, +0.15f, +0.0f,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f  // top left 
};

u32 indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

u32 vertex_components[] = {3, 3, 2};
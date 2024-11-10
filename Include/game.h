#pragma once

#include <ckit.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <ECS.h>

#include <shader.h>
#include <mesh.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void game_update_and_render(double delta_time);
void game_init();
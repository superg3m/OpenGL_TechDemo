#pragma once

#include <vector>
#include <glad/glad.h>

void shader_check_compile_error(unsigned int source_id, const char* path);
GLenum shader_type_from_path(const char* shader_source_path);
unsigned int shader_source_compile(const char* path);
unsigned int create_shader_program(std::vector<const char*> shader_paths);
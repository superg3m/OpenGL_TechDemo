#include <material.hpp>
#include <string>

Material::Material() {
    this->opacity = 1.0f;
    this->color = GM_Vec4(1, 1, 1, 1);
}
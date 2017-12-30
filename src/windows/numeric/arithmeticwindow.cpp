#include "arithmeticwindow.hpp"

#include <windowregistry.hpp>

REGISTER_GENERIC_WINDOW_CLASS(ArithmeticWindow, <CgDataType::UniformFloat>, Menu::Arithmetic, "arithmetic-float", "Float Arithmetic");
REGISTER_GENERIC_WINDOW_CLASS(ArithmeticWindow, <CgDataType::UniformVec2>,  Menu::Arithmetic, "arithmetic-vec2", "Vec2 Arithmetic");
REGISTER_GENERIC_WINDOW_CLASS(ArithmeticWindow, <CgDataType::UniformVec3>,  Menu::Arithmetic, "arithmetic-vec3", "Vec3 Arithmetic");
REGISTER_GENERIC_WINDOW_CLASS(ArithmeticWindow, <CgDataType::UniformVec4>,  Menu::Arithmetic, "arithmetic-vec4", "Vec4 Arithmetic");

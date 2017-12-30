#include "bufferwindow.hpp"
#include <windowregistry.hpp>

REGISTER_GENERIC_WINDOW_CLASS(BufferWindow, <CgDataType::UniformFloat>, Menu::Buffer, "buffer-float", "Float Buffer");
REGISTER_GENERIC_WINDOW_CLASS(BufferWindow, <CgDataType::UniformVec2>, Menu::Buffer, "buffer-vec2", "Vec2 Buffer");
REGISTER_GENERIC_WINDOW_CLASS(BufferWindow, <CgDataType::UniformVec3>, Menu::Buffer, "buffer-vec3", "Vec3 Buffer");
REGISTER_GENERIC_WINDOW_CLASS(BufferWindow, <CgDataType::UniformVec4>, Menu::Buffer, "buffer-vec4", "Vec4 Buffer");

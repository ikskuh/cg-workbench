#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <imgui.h>

enum class CgDataType
{
	None = 0,
	Texture2D,
	Shader,
	Geometry,
	UniformFloat,
	UniformVec2,
	UniformVec3,
	UniformVec4,
	UniformMat3,
	UniformMat4,
};

template<CgDataType _Type>
struct UniformType;

#define MAPTYPE(_CgType, _DataType, _DisplayImpl) \
	template<> struct UniformType<CgDataType::_CgType> { \
		typedef _DataType type; \
		static void Display(_DataType const & value) { \
			_DisplayImpl;\
		} \
	}

MAPTYPE(UniformFloat, float, ImGui::Text("float(%f)", value));
MAPTYPE(UniformVec2, glm::vec2, ImGui::Text("vec2(%f,%f)", value.x, value.y));
MAPTYPE(UniformVec3, glm::vec3, ImGui::Text("vec3(%f,%f,%f)", value.x, value.y, value.z));
MAPTYPE(UniformVec4, glm::vec4, ImGui::Text("vec4(%f,%f,%f,%f)", value.x, value.y, value.z, value.w));
MAPTYPE(UniformMat3, glm::mat3, (void)value; ImGui::Text("mat3(...)"));
MAPTYPE(UniformMat4, glm::mat4, (void)value; ImGui::Text("mat4(...)"));

MAPTYPE(Texture2D, GLuint, ImGui::Image((ImTextureID)uintptr_t(value), ImVec2(128, 128)));
MAPTYPE(Shader, GLuint, (void)value);
MAPTYPE(Geometry, GLuint, (void)value);

#undef MAPTYPE

static inline char const * DisplayName(CgDataType type)
{
	switch(type)
	{
		case CgDataType::Texture2D: return "texture";
		case CgDataType::Geometry: return "geometry";
		case CgDataType::Shader: return "shader";
		case CgDataType::UniformFloat: return "float";
		case CgDataType::UniformVec2: return "vec2";
		case CgDataType::UniformVec3: return "vec3";
		case CgDataType::UniformVec4: return "vec4";
		case CgDataType::UniformMat3: return "mat3";
		case CgDataType::UniformMat4: return "mat4";
		default: return "???";
	}
}

static inline void DisplayDataValue(CgDataType type, void const * value)
{
	switch(type)
	{
		case CgDataType::None: return;
#define X(_Type) \
		case _Type: \
			UniformType<_Type>::Display(*static_cast<typename UniformType<_Type>::type const *>(value)); \
			break
		X(CgDataType::Texture2D);
		X(CgDataType::Geometry);
		X(CgDataType::Shader);
		X(CgDataType::UniformFloat);
		X(CgDataType::UniformVec2);
		X(CgDataType::UniformVec3);
		X(CgDataType::UniformVec4);
		X(CgDataType::UniformMat3);
		X(CgDataType::UniformMat4);
#undef X
	}
}
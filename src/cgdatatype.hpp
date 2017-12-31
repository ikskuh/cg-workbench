#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include "geometry.hpp"
#include "shaderprogram.hpp"
#include "audiostream.hpp"
#include "event.hpp"

enum class CgDataType
{
	None = 0,
	Texture2D,
	Shader,
	Geometry,
	Audio,
	Event, //! can be TRIGGERED!!!
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
MAPTYPE(UniformMat4, glm::mat4, (void)value; ImGui::Text("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f", value[0].x, value[0].y, value[0].z, value[0].w, value[1].x, value[1].y, value[1].z, value[1].w, value[2].x, value[2].y, value[2].z, value[2].w, value[3].x, value[3].y, value[3].z, value[3].w));

MAPTYPE(Texture2D, GLuint, ImGui::Image((ImTextureID)uintptr_t(value), ImVec2(128, 128)));
MAPTYPE(Shader, ShaderProgram, (void)value);
MAPTYPE(Geometry, Geometry, ImGui::Text("%d vertices", value.VertexCount));
MAPTYPE(Audio, AudioStream, ImGui::Text("%d channels", value.GetChannels()));
MAPTYPE(Event, Event, ImGui::Text("Triggered %d times", value.GetCounter()) );

#undef MAPTYPE

static inline char const * DisplayName(CgDataType type)
{
	switch(type)
	{
		case CgDataType::Texture2D: return "texture";
		case CgDataType::Geometry: return "geometry";
		case CgDataType::Shader: return "shader";
		case CgDataType::Audio: return "audio";
		case CgDataType::Event: return "event";
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
		X(CgDataType::Audio);
		X(CgDataType::Event);
		X(CgDataType::UniformFloat);
		X(CgDataType::UniformVec2);
		X(CgDataType::UniformVec3);
		X(CgDataType::UniformVec4);
		X(CgDataType::UniformMat3);
		X(CgDataType::UniformMat4);
#undef X
	}
}

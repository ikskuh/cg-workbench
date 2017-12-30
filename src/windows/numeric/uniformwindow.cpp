#include "uniformwindow.hpp"

using namespace nlohmann;

#include <windowregistry.hpp>
REGISTER_GENERIC_WINDOW_CLASS(UniformWindow, <CgDataType::UniformFloat>, Menu::Values, "uniform-float", "Float");
REGISTER_GENERIC_WINDOW_CLASS(UniformWindow, <CgDataType::UniformVec2>, Menu::Values, "uniform-vec2", "Vec2");
REGISTER_GENERIC_WINDOW_CLASS(UniformWindow, <CgDataType::UniformVec3>, Menu::Values, "uniform-vec3", "Vec3");
REGISTER_GENERIC_WINDOW_CLASS(UniformWindow, <CgDataType::UniformVec4>, Menu::Values, "uniform-vec4", "Vec4");
REGISTER_GENERIC_WINDOW_CLASS(UniformWindow, <CgDataType::UniformMat3>, Menu::Values, "uniform-mat3", "Mat3");
REGISTER_GENERIC_WINDOW_CLASS(UniformWindow, <CgDataType::UniformMat4>, Menu::Values, "uniform-mat4", "Mat4");

template<> char const * UniformWindow<CgDataType::UniformFloat>::GetTitle() { return "Value (float)"; }
template<> char const * UniformWindow<CgDataType::UniformVec2>::GetTitle()  { return "Value (vec2)"; }
template<> char const * UniformWindow<CgDataType::UniformVec3>::GetTitle()  { return "Value (vec3)"; }
template<> char const * UniformWindow<CgDataType::UniformVec4>::GetTitle()  { return "Value (vec4)"; }
template<> char const * UniformWindow<CgDataType::UniformMat3>::GetTitle()  { return "Value (mat3)"; }
template<> char const * UniformWindow<CgDataType::UniformMat4>::GetTitle()  { return "Value (mat4)"; }

static const float speed = 0.01f;

// float

template<> void UniformWindow<CgDataType::UniformFloat>::OnUpdate()
{
	ImGui::DragFloat("Value", &this->value, speed);
}

template<> json UniformWindow<CgDataType::UniformFloat>::Serialize() const
{
	return {
		{ "type", "uniform-float" },
		{ "value", this->value }
	};
}

template<> void UniformWindow<CgDataType::UniformFloat>::Deserialize(const json &value)
{
	this->value = value["value"];
}

// vec2

template<> void UniformWindow<CgDataType::UniformVec2>::OnUpdate()
{
	ImGui::DragFloat2("Value", &this->value.x, speed);
}

template<> json UniformWindow<CgDataType::UniformVec2>::Serialize() const
{
	return {
		{ "type", "uniform-vec2" },
		{ "value", { this->value.x, this->value.y } }
	};
}

template<> void UniformWindow<CgDataType::UniformVec2>::Deserialize(const json &value)
{
	this->value.x = value["value"][0];
	this->value.y = value["value"][1];
}

// vec3

template<> void UniformWindow<CgDataType::UniformVec3>::OnUpdate()
{
	ImGui::DragFloat3("Value", &this->value.x, speed);
}

template<> json UniformWindow<CgDataType::UniformVec3>::Serialize() const
{
	return {
		{ "type", "uniform-vec3" },
		{ "value", { this->value.x, this->value.y, this->value.z } }
	};
}

template<> void UniformWindow<CgDataType::UniformVec3>::Deserialize(const json &value)
{
	this->value.x = value["value"][0];
	this->value.y = value["value"][1];
	this->value.z = value["value"][2];
}

// vec4

template<> void UniformWindow<CgDataType::UniformVec4>::OnUpdate()
{
	ImGui::DragFloat4("Value", &this->value.x, speed);
}

template<> json UniformWindow<CgDataType::UniformVec4>::Serialize() const
{
	return {
		{ "type", "uniform-vec4" },
		{ "value", { this->value.x, this->value.y, this->value.z, this->value.w } }
	};
}

template<> void UniformWindow<CgDataType::UniformVec4>::Deserialize(const json &value)
{
	this->value.x = value["value"][0];
	this->value.y = value["value"][1];
	this->value.z = value["value"][2];
	this->value.w = value["value"][3];
}

// mat3

template<> void UniformWindow<CgDataType::UniformMat3>::OnUpdate()
{
	ImGui::Text("Matrix");
	ImGui::DragFloat3("[0]", &this->value[0].x, speed);
	ImGui::DragFloat3("[1]", &this->value[1].x, speed);
	ImGui::DragFloat3("[2]", &this->value[2].x, speed);
}

template<> json UniformWindow<CgDataType::UniformMat3>::Serialize() const
{
	json array;
	for(int r = 0; r < 3; r++)
		array[r] = { this->value[r].x, this->value[r].y, this->value[r].z };

	return {
		{ "type", "uniform-mat3" },
		{ "value", array }
	};
}

template<> void UniformWindow<CgDataType::UniformMat3>::Deserialize(const json &value)
{
	for(int r = 0; r < 3; r++)
		for(int c = 0; c < 3; c++)
			this->value[r][c] = value["value"][r][c];
}

// mat4

template<> void UniformWindow<CgDataType::UniformMat4>::OnUpdate()
{
	ImGui::Text("Matrix");
	ImGui::DragFloat4("[0]", &this->value[0].x, speed);
	ImGui::DragFloat4("[1]", &this->value[1].x, speed);
	ImGui::DragFloat4("[2]", &this->value[2].x, speed);
	ImGui::DragFloat4("[3]", &this->value[3].x, speed);
}

template<> json UniformWindow<CgDataType::UniformMat4>::Serialize() const
{
	json array;
	for(int r = 0; r < 4; r++)
		array[r] = { this->value[r].x, this->value[r].y, this->value[r].z, this->value[r].w };

	return {
		{ "type", "uniform-mat4" },
		{ "value", array }
	};
}

template<> void UniformWindow<CgDataType::UniformMat4>::Deserialize(const json &value)
{
	for(int r = 0; r < 4; r++)
		for(int c = 0; c < 4; c++)
			this->value[r][c] = value["value"][r][c];
}


#include "colorwindow.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(ColorWindow, Menu::Values, "uniform-color", "Color");

ColorWindow::ColorWindow() :
    Window("Color", ImGuiWindowFlags_AlwaysAutoResize),
    color(0.8f, 0.8f, 0.8f, 1.0f)
{
	this->AddSource(new Source(CgDataType::UniformVec3, "RGB", &this->color.r));
	this->AddSource(new Source(CgDataType::UniformVec4, "RGBA", &this->color.r));
}

ColorWindow::~ColorWindow()
{

}

void ColorWindow::OnUpdate()
{
	ImGui::ColorPicker4("Color", &this->color.x, ImGuiColorEditFlags_AlphaPreviewHalf);
}


nlohmann::json ColorWindow::Serialize() const
{
	return {
		{ "type", ColorWindowID },
		{ "color", { this->color.x, this->color.y, this->color.z, this->color.w } }
	};
}

void ColorWindow::Deserialize(const nlohmann::json &value)
{
	this->color.x = value["color"][0];
	this->color.y = value["color"][1];
	this->color.z = value["color"][2];
	this->color.w = value["color"][3];
}

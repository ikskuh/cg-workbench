#include "trigger.hpp"

Trigger::Trigger() :
    Window("Trigger", ImGuiWindowFlags_AlwaysAutoResize)
{
	this->AddSource(new Source(CgDataType::UniformFloat, "Value", &this->value));
}

void Trigger::OnUpdate()
{
	this->value = ImGui::Button("Fire!") ? 1.0f : 0.0f;
}

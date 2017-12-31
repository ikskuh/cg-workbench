#include "trigger.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(Trigger, Menu, "trigger", "Trigger")

Trigger::Trigger() :
    Window("Trigger", ImGuiWindowFlags_AlwaysAutoResize)
{
	this->AddSource(new Source(CgDataType::UniformFloat, "Value", &this->value));
	this->AddSource(new Source(CgDataType::Event, "Event", &this->triggered));
}

void Trigger::OnUpdate()
{
	this->value = this->triggered = ImGui::Button(this->GetTitle().c_str()) ? 1.0f : 0.0f;
}

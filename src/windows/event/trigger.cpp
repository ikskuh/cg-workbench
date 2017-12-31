#include "trigger.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(Trigger, Menu::Event, "trigger", "Trigger")

Trigger::Trigger() :
    Window("Trigger", ImGuiWindowFlags_AlwaysAutoResize)
{
	this->triggered = this->CreateEvent();
	this->AddSource<CgDataType::UniformFloat>("Value", &this->value);
	this->AddSource<CgDataType::Event>("Event", this->triggered);
}

void Trigger::OnUpdate()
{
	if((this->value = ImGui::Button(this->GetTitle().c_str())) > 0.0f)
	{
		this->triggered->Trigger();
	}
}

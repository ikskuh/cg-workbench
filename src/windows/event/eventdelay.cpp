#include "eventdelay.hpp"

#include <SDL.h>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(EventDelay, Menu::Event, "event-delay", "Event Delay")

EventDelay::EventDelay() :
    Window("Event Delay", ImGuiWindowFlags_AlwaysAutoResize),
    delayedEvents(),
    delay(100)
{
	this->output = this->CreateEvent();

	this->input = this->AddSink<CgDataType::Event>("Input");

	this->AddSource<CgDataType::Event>("Output", this->output);
}

void EventDelay::OnUpdate()
{
	ImGui::DragInt("Delay", &this->delay, 1.0, 0, 1000000);

	if(this->input->GetObject<CgDataType::Event>())
		this->delayedEvents.emplace(SDL_GetTicks() + this->delay);

	while(this->delayedEvents.size() > 0 && this->delayedEvents.front() <= SDL_GetTicks())
	{
		this->output->Trigger();
		this->delayedEvents.pop();
	}
}

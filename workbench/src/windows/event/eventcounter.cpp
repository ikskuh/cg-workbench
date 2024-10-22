#include "eventcounter.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(EventCounter, Menu::Event, "event-counter", "Event Counter")

EventCounter::EventCounter() :
    Window("Event Counter", ImGuiWindowFlags_AlwaysAutoResize),
    counter(0)
{
	this->up = this->AddSink<CgDataType::Event>("+", Sink::UnlimitedConnections);
	this->down = this->AddSink<CgDataType::Event>("-", Sink::UnlimitedConnections);
	this->reset = this->AddSink<CgDataType::Event>("0", Sink::UnlimitedConnections);
	this->AddSource<CgDataType::UniformFloat>("Count", &this->counter);
}

void EventCounter::OnRender()
{
	this->counter += Event::Count(this->up);
	this->counter -= Event::Count(this->down);
	if(Event::Any(this->reset))
		this->counter = 0;
}

void EventCounter::OnUpdate()
{
	ImGui::Text("Count = %d", (int)(this->counter));
}

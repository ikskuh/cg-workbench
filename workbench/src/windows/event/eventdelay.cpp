#include "eventdelay.hpp"
#include "time.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(EventDelay, Menu::Event, "event-delay", "Event Delay")

EventDelay::EventDelay() :
    Window("Event Delay", ImGuiWindowFlags_AlwaysAutoResize),
    delayedEvents(),
    delay(100),
    suppressor(0),
    lastEvent(0)
{
	this->output = this->CreateEvent();

	this->input = this->AddSink<CgDataType::Event>("Input", -1);

	this->AddSource<CgDataType::Event>("Output", this->output);
}

void EventDelay::OnUpdate()
{
	ImGui::DragInt("Delay", &this->delay, 1.0, 0, INT_MAX);

	ImGui::DragInt("Suppression", &this->suppressor, 1.0, 0, INT_MAX);

	if(Event::Any(this->input) && (lastEvent <= (int(Time::get()) - this->suppressor)))
	{
		this->delayedEvents.emplace(Time::get() + this->delay / 1000.0f);
		lastEvent = Time::get();
	}

	while(this->delayedEvents.size() > 0 && this->delayedEvents.front() <= Time::get())
	{
		this->output->Trigger();
		this->delayedEvents.pop();
	}
}

WINDOW_SERIALIZE_IMPL(EventDelay)
{
	return {
		{ "delay", this->delay },
		{ "suppressor", this->suppressor }
	};
}

WINDOW_DESERIALIZE_IMPL(EventDelay)
{
	this->delay = data.value("delay", 100);
	this->suppressor = data.value("suppressor", 0);
}

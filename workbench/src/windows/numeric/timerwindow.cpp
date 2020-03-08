#include "timerwindow.hpp"
#include "time.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(TimerWindow, Menu::Values, "timer", "Time")

TimerWindow::TimerWindow() :
    Window("Timer", ImGuiWindowFlags_AlwaysAutoResize),
    total(0.0),
    delta(0.0),
    speed(1.0),
    stamp(Time::get())
{
	this->reset = this->AddSink<CgDataType::Event>("Reset", Sink::UnlimitedConnections);
	this->AddSource(new Source(CgDataType::UniformFloat, "Total", &this->total));
	this->AddSource(new Source(CgDataType::UniformFloat, "Delta", &this->delta));
}

TimerWindow::~TimerWindow()
{

}

void TimerWindow::OnRender()
{
	float current = Time::get();

	this->delta = (current - this->stamp);
	this->total += this->speed * this->delta;

	if(Event::Any(this->reset))
		this->total = 0;

	this->stamp = current;
}

void TimerWindow::OnUpdate()
{
	ImGui::Text("Current = %f", double(this->total));

	if(ImGui::Button("Reset")) this->total = 0;

	ImGui::DragFloat("Speed", &this->speed, 0.01f);
}

nlohmann::json TimerWindow::Serialize() const
{
	return {
		{ "current", this->total },
		{ "speed", this->speed }
	};
}

void TimerWindow::Deserialize(const nlohmann::json &value)
{
	this->speed = value["speed"];
	this->total = value["current"];
}

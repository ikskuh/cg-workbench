#include "pulsenode.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(PulseNode, Menu::Event, "event-pulse", "Pulse Node")

PulseNode::PulseNode() :
    Window("Pulse Node", ImGuiWindowFlags_AlwaysAutoResize)
{
	this->trigger = this->AddSink<CgDataType::Event>("Trigger", Sink::UnlimitedConnections);
	this->AddSource<CgDataType::UniformFloat>("Output", &this->output);
}

void PulseNode::OnRender()
{
	this->output = Event::Any(this->trigger);
}

void PulseNode::OnUpdate()
{
	ImGui::Text("Does not actually generate\n"
				"a dirac impulse");
}

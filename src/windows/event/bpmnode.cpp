#include "bpmnode.hpp"

#include <SDL.h>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(BPMNode, Menu::Event, "event-bpm", "BPM Node")

BPMNode::BPMNode() :
    Window("BPM Node", ImGuiWindowFlags_AlwaysAutoResize),
    ticking(false),
    frequency(60.0),
    offset(0)
{
	this->tick = this->CreateEvent();

	this->start = this->AddSink<CgDataType::Event>("Start");
	this->stop  = this->AddSink<CgDataType::Event>("Stop");

	this->AddSource<CgDataType::Event>("Tick", this->tick);
}

void BPMNode::OnUpdate()
{
	int ticks = SDL_GetTicks();
	if((ticks - this->offset) > this->timer)
	{
		if(this->ticking)
			this->tick->Trigger();
		this->timer += (60000 / this->frequency);
	}

	ImGui::DragInt("BPM", &this->frequency, 1, 1, 1000); // 1000 BPM!

	ImGui::DragInt("Offset", &this->offset, 1, 0, (60000 / this->frequency));


	if(ImGui::Button("Start") || this->start->GetObject<CgDataType::Event>())
		this->ticking = true;

	ImGui::SameLine();

	if(ImGui::Button("Stop") || this->stop->GetObject<CgDataType::Event>())
		this->ticking = true;

	ImGui::Separator();
}

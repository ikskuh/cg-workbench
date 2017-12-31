#include "bpmnode.hpp"

#include <SDL.h>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(BPMNode, Menu::Event, "event-bpm", "BPM Node")

BPMNode::BPMNode() :
    Window("BPM Node", ImGuiWindowFlags_AlwaysAutoResize),
    ticking(false),
    frequency(60.0)
{
	this->tick = this->CreateEvent();

	this->start = this->AddSink<CgDataType::Event>("Start", -1);
	this->stop  = this->AddSink<CgDataType::Event>("Stop", -1);

	this->AddSource<CgDataType::Event>("Tick", this->tick);
}

void BPMNode::OnUpdate()
{
	int ticks = SDL_GetTicks();
	if(ticks > this->timer)
	{
		if(this->ticking)
			this->tick->Trigger();
		this->timer += (60000 / this->frequency);
	}

	ImGui::DragInt("BPM", &this->frequency, 1, 1, 1000); // 1000 BPM!

	if(ImGui::Button("Start") || Event::Any(this->start))
	{
		this->timer = SDL_GetTicks();
		this->ticking = true;
	}

	ImGui::SameLine();

	if(ImGui::Button("Stop") || Event::Any(this->stop))
		this->ticking = false;

	ImGui::SameLine();

	if(ImGui::Button("Tap me!"))
	{
		this->ticking = true;
		this->timer = SDL_GetTicks(); // Sync with us!

		for(int i = 1; i < 4; i++)
			this->lastTaps[i - 1] = this->lastTaps[i];
		this->lastTaps[3] = SDL_GetTicks();

		int delta = 0;
		for(int i = 0; i < 3; i++)
			delta += (this->lastTaps[i+1] - this->lastTaps[i+0]);
		delta /= 3;

		if(delta > 0)
		{
			this->frequency = 60000 / delta;
		}
	}
}

#include "mergechannelsnode.hpp"
#include <cassert>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(MergeChannelsNode, Menu::Audio, "audio-merge-channels", "Merge Channels")

MergeChannelsNode::MergeChannelsNode() :
    AudioNode("Channel Merger", ImGuiWindowFlags_AlwaysAutoResize),
    output()
{
	this->input = this->AddSink<CgDataType::Audio>("Input", -1);
	this->AddSource(new Source(CgDataType::Audio, "Output", &this->output));
}

void MergeChannelsNode::OnRenderAudio()
{
	this->output.SetChannels(std::max(this->input->GetSourceCount(), 1));
	for(int i = 0; i < this->output.GetLength(); i++)
		for(int j = 0; j < this->input->GetSourceCount(); j++)
			this->output.Sample(i, j) = this->input->GetObject<CgDataType::Audio>(j).Sample(i, 0);
}

void MergeChannelsNode::OnUpdate()
{
	ImGui::Text("Merging channels");
}


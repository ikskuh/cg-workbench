#include "mergechannelsnode.hpp"
#include <cassert>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(MergeChannelsNode, Menu::Audio, "audio-merge-channels", "Merge Channels")

MergeChannelsNode::MergeChannelsNode() :
    AudioNode("Channel Merger", ImGuiWindowFlags_AlwaysAutoResize),
    inputs(),
    output()
{
	this->Resize(1);
	this->AddSource(new Source(CgDataType::Audio, "Output", &this->output));
}

void MergeChannelsNode::OnRenderAudio()
{
	for(int i = 0; i < this->output.GetLength(); i++)
	{
		for(int j = 0; j < this->output.GetChannels(); j++)
		{
			this->output.Sample(i, j) = this->inputs[i]->GetObject<CgDataType::Audio>().Sample(i, 0);
		}
	}
}

void MergeChannelsNode::OnUpdate()
{
	int chans = this->inputs.size();
	if(ImGui::DragInt("Channels", &chans, 0.1f, 1, 10))
		this->Resize(chans);
}


void MergeChannelsNode::Resize(int chans)
{
	assert(chans >= 1);
	if(this->inputs.size() == size_t(chans))
		return;

	while(this->inputs.size() < size_t(chans))
	{
		char buffer[64];
		int cnt = this->GetSourceCount();
		snprintf(buffer, 64, "Channel %d", cnt);

		Sink * sink = new Sink(CgDataType::Audio, buffer);
		this->AddSink(sink);
		this->inputs.emplace_back(sink);
	}
	while(this->inputs.size() > size_t(chans))
	{
		this->RemoveSink(this->inputs.back());
		this->inputs.pop_back();
	}
	this->output.SetChannels(chans);
}

nlohmann::json MergeChannelsNode::Serialize() const
{
	return {
		{ "channels", this->output.GetChannels() }
	};
}

void MergeChannelsNode::Deserialize(nlohmann::json const & value)
{
	this->Resize(value.value("channels", 1));
}

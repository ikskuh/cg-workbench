#include "splitchannelsnode.hpp"

#include "windowregistry.hpp"
REGISTER_WINDOW_CLASS(SplitChannelsNode, Menu::Audio, "audio-split-channels", "Split Channels")

SplitChannelsNode::SplitChannelsNode() :
    AudioNode("Split Channels", ImGuiWindowFlags_AlwaysAutoResize),
    sourceStream(nullptr),
    targetStreams()
{
	this->AddSink(this->sourceStream = new Sink(CgDataType::Audio, "Source"));
}

void SplitChannelsNode::OnRenderAudio()
{

}

void SplitChannelsNode::OnUpdate()
{
	AudioStream const & source = this->sourceStream->GetObject<CgDataType::Audio>();

	while(this->targetStreams.size() < size_t(source.GetChannels()))
	{
		char buffer[64];
		int cnt = this->GetSourceCount();
		snprintf(buffer, 64, "Channel %d", cnt);
		this->targetStreams.emplace_back(1);
		this->AddSource(new Source(CgDataType::Audio, buffer, &this->targetStreams.back()));
	}

	while(size_t(this->GetSourceCount()) > size_t(source.GetChannels()))
	{
		this->RemoveSource(this->GetSource(this->GetSourceCount() - 1));
		this->targetStreams.pop_back();
	}

	ImGui::Text("%d Channels", int(this->targetStreams.size()));
}

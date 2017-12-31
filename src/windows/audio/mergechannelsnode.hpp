#pragma once

#include "audionode.hpp"

class MergeChannelsNode :
	public AudioNode
{
	WINDOW_PREAMBLE
	WINDOW_CUSTOM_SERIALIZE
private:
	std::vector<Sink*> inputs;
	AudioStream output;

	void Resize(int chans);
protected:
	void OnRenderAudio() override;
	void OnUpdate() override;
public:
	MergeChannelsNode();
};

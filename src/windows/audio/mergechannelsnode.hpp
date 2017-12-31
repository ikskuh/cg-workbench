#pragma once

#include "audionode.hpp"

class MergeChannelsNode :
	public AudioNode
{
	WINDOW_PREAMBLE
private:
	Sink * input;
	AudioStream output;
protected:
	void OnRenderAudio() override;
	void OnUpdate() override;
public:
	MergeChannelsNode();
};

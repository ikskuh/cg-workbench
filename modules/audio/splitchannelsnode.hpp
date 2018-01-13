#pragma once

#include "audionode.hpp"

class SplitChannelsNode :
	public AudioNode
{
	WINDOW_PREAMBLE
private:
	Sink * sourceStream;
	std::vector<AudioStream> targetStreams;
protected:
	void OnRenderAudio();
	void OnUpdate();
public:
	SplitChannelsNode();
};

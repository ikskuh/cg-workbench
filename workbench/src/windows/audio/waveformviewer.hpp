#pragma once

#include "audionode.hpp"

class WaveformViewer :
	public AudioNode
{
	WINDOW_PREAMBLE
private:
	Sink * input;
	AudioStream data;
	bool takeSample;
	bool liveMode;
protected:
	void OnUpdate() override;
	void OnRenderAudio() override;
public:
	WaveformViewer();
};

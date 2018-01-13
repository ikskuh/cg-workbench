#pragma once

#include "audionode.hpp"

class SynthNode :
	public AudioNode
{
	WINDOW_PREAMBLE
	WINDOW_CUSTOM_SERIALIZE
private:
	AudioStream stream;

	float phase;
	float dutyCycle;
	float freq;
	int waveform;

protected:
	void OnRenderAudio() override;
	void OnUpdate() override;
public:
	SynthNode();
};

#pragma once

#include "audionode.hpp"
#include <pulse/simple.h>

class PulseMicrophone :
	public AudioNode
{
	WINDOW_PREAMBLE
private:
	AudioStream stream;
	pa_simple * pulse;
protected:
	void OnUpdate() override;

	void OnRenderAudio() override;
public:
	PulseMicrophone();
	~PulseMicrophone();
};

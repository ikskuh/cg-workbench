#pragma once

#include "audionode.hpp"

class AudioOutput :
	public AudioNode
{
	WINDOW_PREAMBLE
	WINDOW_CUSTOM_SERIALIZE
private:
	Sink * stream;
	std::vector<float> volumes;
	bool muted;
protected:
	void OnUpdate() override;

	void OnRenderAudio() override;
public:
	AudioOutput();
};

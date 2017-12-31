#pragma once

#include "audionode.hpp"

#include <stb_vorbis.h>

class SoundFile :
	public AudioNode
{
	WINDOW_PREAMBLE
	WINDOW_CUSTOM_SERIALIZE
private:
	AudioStream stream;
	float offset;
	bool finished;
	std::string soundFile;
	stb_vorbis * vorbis;

	Sink * restart;
	Sink * start;
	Sink * pause;
	Sink * stop;

	bool playing;

	void LoadOgg(std::string name);
protected:
	void OnRenderAudio() override;
	void OnUpdate() override;
public:
	SoundFile();
	~SoundFile();
};

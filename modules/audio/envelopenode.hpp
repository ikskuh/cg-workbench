#ifndef ENVELOPENODE_HPP
#define ENVELOPENODE_HPP

#include "audionode.hpp"

class EnvelopeNode :
	public AudioNode
{
	WINDOW_PREAMBLE
	WINDOW_CUSTOM_SERIALIZE
private:
	enum EnvelopeState {
		Disabled,
		Attack,
		Decay,
		Sustain,
		Release
	};

	AudioStream output;

	Sink * input;
	Sink * start;
	Sink * end;

	float delta_time;
	EnvelopeState state;

	bool is_attacked;

	float attack, decay, sustain, release;

protected:
	void OnRenderAudio() override;
	void OnUpdate() override;
public:
	EnvelopeNode();
};

#endif // ENVELOPENODE_HPP

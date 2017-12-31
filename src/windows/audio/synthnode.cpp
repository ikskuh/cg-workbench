#include "synthnode.hpp"

#include <cmath>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(SynthNode, Menu::Audio, "audio-synth", "Synthesizer")

SynthNode::SynthNode() :
    AudioNode("Synthesizer"),
    stream(1),
    phase(0.0),
    dutyCycle(0.5),
    freq(440),
    waveform(0)
{
	this->AddSource<CgDataType::Audio>("Stream", &this->stream);
}

void SynthNode::OnRenderAudio()
{
	float phaseIncrement = this->freq / audio_samplerate;
	for(int i = 0; i < stream.GetLength(); i++)
	{
		float v = 0;
		switch(this->waveform)
		{
			case 0: v = sin(2.0 * M_PI * this->phase); break;
			case 1: v = (this->phase >= this->dutyCycle) ? 1.0 : -1.0; break;
			case 2:
				if(this->phase < this->dutyCycle)
					v = 2.0 * (this->phase / this->dutyCycle) - 1.0;
				else
					v = 1.0 - 2.0 * ((this->phase - this->dutyCycle) / (1.0 - this->dutyCycle));
				break;
			case 3: v = 2.0 * (rand() / float(RAND_MAX)) - 1.0; break;
		}
		stream.Sample(i,0) = v;
		this->phase = glm::fract(this->phase + phaseIncrement);
	}
}

void SynthNode::OnUpdate()
{
	ImGui::DragFloat("Frequency", &this->freq, 1.0f, 10.0f, 20000.0f);

	ImGui::SliderFloat("Duty Cycle", &this->dutyCycle, 0.0f, 1.0f);

	if(ImGui::RadioButton("Sine Wave", (this->waveform == 0))) this->waveform = 0;
	if(ImGui::RadioButton("Square Wave", (this->waveform == 1))) this->waveform = 1;
	if(ImGui::RadioButton("Triangle Wave", (this->waveform == 2))) this->waveform = 2;
	if(ImGui::RadioButton("Noise", (this->waveform == 3))) this->waveform = 3;
}

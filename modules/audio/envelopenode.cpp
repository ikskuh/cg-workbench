#include "envelopenode.hpp"


#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(EnvelopeNode, Menu::Audio, "audio-envelope", "Envelope Generator")

EnvelopeNode::EnvelopeNode() :
    AudioNode("Envelope Generator"),
    output(1)
{
	input = this->AddSink<CgDataType::Audio>("Stream");
	start = this->AddSink<CgDataType::Event>("Attack");
	end = this->AddSink<CgDataType::Event>("Release");
	this->AddSource<CgDataType::Audio>("Stream", &this->output);
}

void EnvelopeNode::OnRenderAudio()
{
	if(not input->HasSourceConnected())
		return;

	auto const & stream = input->GetObject<CgDataType::Audio>();

	// float phaseIncrement = this->freq / audio_samplerate;

	this->output.SetChannels(stream.GetChannels());
	for(int i = 0; i < output.GetLength(); i++)
	{
		float f;
		float spd;
		switch(state)
		{
			case Disabled:
				spd = 0.0f;
				f = 0.0f;
				delta_time = 0.0f;
				if(is_attacked)
					state = Attack;
				break;

			case Attack:
				spd = attack;
				if(delta_time >= 1.0f) {
					delta_time -= 1.0f;
					f = 1.0f;
					state = Decay;
				} else {
					f = delta_time;
				}
				break;

			case Decay:
				spd = decay;
				if(delta_time >= 0.3f) {
					delta_time -= 0.3f;
					f = 0.7f;
					state = Sustain;
				} else {
					f = 1.0f - delta_time;
				}

				break;

			case Sustain:
				spd = sustain;
				f = 0.7f;
				delta_time = 0.0f;
				if(is_attacked)
					break;
				state = Release;
				break;

			case Release:
				spd = release;
				if(delta_time >= 0.7f)
				{
					f = 0.0f;
					state = Disabled;
				}
				else {
					f = 0.7f - delta_time;
				}
				break;
		}

		delta_time += spd / audio_samplerate;

		for(int j = 0; j < stream.GetChannels(); j++)
			this->output.Sample(i, j) = f * stream.Sample(i, 0);
	}
}

void EnvelopeNode::OnUpdate()
{
	if(Event::Any(start))
		is_attacked = true;
	if(Event::Any(end))
		is_attacked = false;

	ImGui::DragFloat("Attack", &attack, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Decay", &decay, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Sustain", &sustain, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Release", &release, 0.01f, 0.0f, 10.0f);
}

WINDOW_SERIALIZE_IMPL(EnvelopeNode)
{
	return {
	};
}

WINDOW_DESERIALIZE_IMPL(EnvelopeNode)
{
	(void)data;
}

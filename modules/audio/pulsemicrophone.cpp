#include "pulsemicrophone.hpp"

#include "windowregistry.hpp"
REGISTER_WINDOW_CLASS(PulseMicrophone, Menu::Audio, "audio-pulse-mic", "Microphone")


PulseMicrophone::PulseMicrophone() :
    AudioNode("Microphone", ImGuiWindowFlags_AlwaysAutoResize),
    stream(2)
{
	pa_sample_spec ss;
	ss.format = PA_SAMPLE_FLOAT32NE;
	ss.channels = 2;
	ss.rate = audio_samplerate;

	this->pulse = pa_simple_new(NULL,               // Use the default server.
	                  "CG Workbench",     // Our application's name.
	                  PA_STREAM_RECORD,
	                  NULL,               // Use the default device.
	                  "Microphone",       // Description of our stream.
	                  &ss,                // Our sample format.
	                  NULL,               // Use default channel map
	                  NULL,               // Use default buffering attributes.
	                  NULL                // Ignore error code.
	                  );
	assert(this->pulse);

	this->AddSource<CgDataType::Audio>("Stream", &this->stream);
}

PulseMicrophone::~PulseMicrophone()
{
	pa_simple_free(this->pulse);
}

void PulseMicrophone::OnRenderAudio()
{
	pa_simple_read(
					this->pulse,
		this->stream.GetData(),
		this->stream.GetLength() * this->stream.GetChannels() * sizeof(sample_t),
		nullptr);
}

void PulseMicrophone::OnUpdate()
{
	ImGui::Text("Pulse Audio Sound Input");
}

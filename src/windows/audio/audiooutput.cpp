#include "audiooutput.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(AudioOutput, Menu::Audio, "audio-output", "Output")

AudioOutput::AudioOutput() :
    AudioNode("Output", ImGuiWindowFlags_AlwaysAutoResize)
{
	this->AddSink(this->stream = new Sink(CgDataType::Audio, "Stream"));
}

void AudioOutput::OnUpdate()
{
	AudioStream const & stream = this->stream->GetObject<CgDataType::Audio>();

	this->volumes.resize(stream.GetChannels());

	for(size_t i = 0; i < this->volumes.size(); i++)
	{
		if(i > 0)
			ImGui::SameLine();
		ImGui::PushID(i);
		ImGui::VSliderFloat(
			"",
			ImVec2(32, 128),
			&this->volumes[i],
			0.0f,
			1.0f,
			"%.2f");
		ImGui::PopID();
	}
}

extern sample_t * audio_destbuffer;

void AudioOutput::OnRenderAudio()
{
	auto const & stream = this->stream->GetObject<CgDataType::Audio>();
	for(int i = 0; i < audio_buffersize; i++)
	{
		for(int j = 0; j < std::min(stream.GetChannels(), audio_channels); j++)
			audio_destbuffer[audio_channels * i + j] += this->volumes[j] * stream.Sample(i, j);
	}
}


nlohmann::json AudioOutput::Serialize() const
{
	nlohmann::json volumes;
	for(size_t i = 0; i < this->volumes.size(); i++)
		volumes[i] = this->volumes[i];

	return {
		{ "muted", this->muted },
		{ "volumes", volumes },
	};
}

void AudioOutput::Deserialize(nlohmann::json const & value)
{
	this->muted = value.value("muted", false);
	try
	{
		nlohmann::json volumes = value.at("volumes");
		if(volumes.is_array())
		{
			for(size_t i = 0; i < std::min(this->volumes.size(), volumes.size()); i++)
				this->volumes[i] = volumes[i];
		}
	}
	catch(nlohmann::detail::exception const & ex) { }
}

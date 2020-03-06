#include "soundfile.hpp"
#include "fileio.hpp"

#include <iostream>

#include "windowregistry.hpp"
REGISTER_WINDOW_CLASS(SoundFile, Menu::Audio, "audio-sound-file", "Sound File")

SoundFile::SoundFile() :
    AudioNode("Sound File", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar),
    stream(1), offset(0.0f),
    soundFile(""),
    vorbis(nullptr),
    playing(false), donePlaying(false)
{
	this->start = this->AddSink<CgDataType::Event>("Start", -1);
	this->pause = this->AddSink<CgDataType::Event>("Pause", -1);
	this->stop = this->AddSink<CgDataType::Event>("Stop", -1);
	this->restart = this->AddSink<CgDataType::Event>("Restart", -1);

	this->finished = this->CreateEvent();

	this->AddSource<CgDataType::Audio>("Sound", &this->stream);
	this->AddSource<CgDataType::UniformFloat>("Offset", &this->offset);
	this->AddSource<CgDataType::Event>("Finished", this->finished);
}

SoundFile::~SoundFile()
{
	if(this->vorbis != nullptr)
		stb_vorbis_close(this->vorbis);
}

void SoundFile::OnRenderAudio()
{
	this->stream.Clear();
	if(this->vorbis == nullptr || this->playing == false)
	{
		return;
	}
	int len = stb_vorbis_get_samples_float_interleaved(
			this->vorbis,
			this->stream.GetChannels(),
			this->stream.GetData(),
			this->stream.GetChannels() * this->stream.GetLength());
	if(len == 0)
		this->donePlaying = true;

	stb_vorbis_info info = stb_vorbis_get_info(this->vorbis);
	unsigned int offset = stb_vorbis_get_sample_offset(this->vorbis);

	this->offset = float(offset / info.channels) / float(info.sample_rate);
}

void SoundFile::OnUpdate()
{
	if(this->donePlaying && this->playing)
	{
		this->finished->Trigger();
		this->playing = false;
		this->donePlaying = false;
	}

	bool rewind = false;
	if(Event::Any(this->restart))
	{
		rewind = true;
		this->playing = true;
	}
	if(Event::Any(this->start))
		this->playing = true;
	if(Event::Any(this->stop))
	{
		this->playing = false;
		rewind = true;
	}
	if(Event::Any(this->pause))
		this->playing = false;

	if(rewind && this->vorbis != nullptr)
		stb_vorbis_seek_start(this->vorbis);

	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Sound File"))
		{
			if(ImGui::MenuItem("Load..."))
			{
				auto path = FileIO::OpenDialog("ogg");
				if(!path.empty())
					this->LoadOgg(path);
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("OGG Player");

	if(this->vorbis)
	{
		unsigned int offset = stb_vorbis_get_sample_offset(this->vorbis);

		ImGui::Text("Samples: %u", offset);
		ImGui::Text("Seconds: %f", this->offset);

	}
}

void SoundFile::LoadOgg(std::string name)
{
	auto * ogg = stb_vorbis_open_filename(name.c_str(), nullptr, nullptr);
	if(ogg == nullptr) {
		std::cerr << "failed to load ogg file: " << name << std::endl;
		return;
	}
	if(this->vorbis != nullptr)
		stb_vorbis_close(this->vorbis);
	this->vorbis = ogg;

	stb_vorbis_info info = stb_vorbis_get_info(ogg);

	printf("freq = %d\n", info.sample_rate);
	printf("chan = %d\n", info.channels);

	this->stream.SetChannels(info.channels);

	this->soundFile = name;

	this->playing = false;

	stb_vorbis_seek_start(ogg);
}

nlohmann::json SoundFile::Serialize() const
{
	return {
		{ "fileName", this->soundFile }
	};
}

void SoundFile::Deserialize(nlohmann::json const & value)
{
	auto path = value.value("fileName", std::string(""));
	if(!path.empty())
		this->LoadOgg(path);
}


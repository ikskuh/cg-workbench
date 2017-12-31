#include "soundfile.hpp"

#include "fileio.hpp"

#include "windowregistry.hpp"
REGISTER_WINDOW_CLASS(SoundFile, Menu::Audio, "audio-sound-file", "Sound File")

SoundFile::SoundFile() :
    AudioNode("Sound File", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar),
    stream(1), offset(0.0f),
    finished(false),
    soundFile(""),
    vorbis(nullptr),
    playing(false)
{
	this->start = this->AddSink<CgDataType::Event>("Start");
	this->pause = this->AddSink<CgDataType::Event>("Pause");
	this->stop = this->AddSink<CgDataType::Event>("Stop");
	this->restart = this->AddSink<CgDataType::Event>("Restart");

	this->AddSource<CgDataType::Audio>("Sound", &this->stream);
	this->AddSource<CgDataType::UniformFloat>("Offset", &this->offset);
	this->AddSource<CgDataType::Event>("Finished", &this->finished);
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
	stb_vorbis_get_samples_float_interleaved(
			this->vorbis,
			this->stream.GetChannels(),
			this->stream.GetData(),
			this->stream.GetChannels() * this->stream.GetLength());

	stb_vorbis_info info = stb_vorbis_get_info(this->vorbis);
	unsigned int offset = stb_vorbis_get_sample_offset(this->vorbis);

	this->offset = float(offset / info.channels) / float(info.sample_rate);
}

void SoundFile::OnUpdate()
{
	// TODO: Implement 'finished' event

	bool rewind = false;
	if(this->restart->GetObject<CgDataType::Event>())
	{
		rewind = true;
		this->playing = true;
	}
	if(this->start->GetObject<CgDataType::Event>())
		this->playing = true;
	if(this->stop->GetObject<CgDataType::Event>())
	{
		this->playing = false;
		rewind = true;
	}
	if(this->pause->GetObject<CgDataType::Event>())
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
	if(ogg == nullptr)
		return;
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


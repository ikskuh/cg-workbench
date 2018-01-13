#include "audiostream.hpp"
#include <cstring>
#include <cassert>

// Implement stb_vorbis here!
#undef STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.h>

int audio_samplerate;
int audio_buffersize;
int audio_channels;


AudioStream::AudioStream(int channels) :
    samples(audio_buffersize * channels),
    channels(channels)
{
	this->Clear();
}

AudioStream::AudioStream(AudioStream const &other) :
    samples(other.samples),
    channels(other.channels)
{

}

AudioStream::AudioStream(AudioStream && other) :
    samples(std::move(other.samples)),
    channels(other.channels)
{

}

AudioStream::~AudioStream()
{

}

void AudioStream::Clear()
{
	memset(this->samples.data(), 0, sizeof(sample_t) * this->samples.size());
}

void AudioStream::SetFormatForStream(AudioStream const & other)
{
	this->SetChannels(other.GetChannels());
}


void AudioStream::SetChannels(int chans)
{
	assert(chans >= 1);
	if(this->channels == chans)
		return;
	this->channels = chans;
	this->samples.resize(audio_buffersize * this->channels);
	this->Clear();
}

void AudioStream::CopyTo(AudioStream & target) const
{
	target.channels = this->channels;
	target.samples = this->samples;
}

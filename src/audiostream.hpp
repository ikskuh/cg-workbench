#pragma once

#include <vector>

extern int audio_samplerate;

extern int audio_buffersize;

extern int audio_channels;

typedef float sample_t;

class AudioStream
{
private:
	std::vector<sample_t> samples;
	int channels;
public:
	explicit AudioStream(int channels = 1);
	AudioStream(AudioStream const &);
	AudioStream(AudioStream &&);
	~AudioStream();

	//! Clears all data.
	//! @remarks Clears all data!
	void Clear();

	//! Resizes this stream for another channel count
	//! @remarks Clears all data!
	void SetChannels(int chans);

	//! Resizes this stream for another channel count
	//! @remarks Clears all data!
	void SetFormatForStream(AudioStream const & other);

	int GetChannels() const { return this->channels; }

	int GetLength() const { return this->samples.size() / this->channels; }

	void CopyTo(AudioStream & target) const;

	sample_t * GetData() { return this->samples.data(); }
	sample_t const * GetData() const { return this->samples.data(); }

	sample_t & Sample(int idx, int channel) { return this->samples[idx * this->channels + channel]; }
	sample_t const & Sample(int idx, int channel) const { return this->samples[idx * this->channels + channel]; }

	sample_t & operator [] (int idx) { return this->samples[idx]; }
	sample_t const & operator [] (int idx) const { return this->samples[idx]; }
};

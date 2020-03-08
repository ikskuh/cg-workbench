#pragma once

#include <vector>
#include <cstdint>

extern uint32_t audio_samplerate;

extern uint32_t audio_buffersize;

extern uint32_t audio_channels;

typedef float sample_t;

class AudioStream
{
private:
	std::vector<sample_t> samples;
	uint32_t channels;
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
	void SetChannels(uint32_t chans);

	//! Resizes this stream for another channel count
	//! @remarks Clears all data!
	void SetFormatForStream(AudioStream const & other);

	uint32_t GetChannels() const { return this->channels; }

    uint32_t GetLength() const { return uint32_t(this->samples.size()) / this->channels; }

	void CopyTo(AudioStream & target) const;

	sample_t * GetData() { return this->samples.data(); }
	sample_t const * GetData() const { return this->samples.data(); }

	sample_t & Sample(size_t idx, uint32_t channel) { return this->samples[idx * this->channels + channel]; }
	sample_t const & Sample(size_t idx, uint32_t channel) const { return this->samples[idx * this->channels + channel]; }

	sample_t & operator [] (size_t idx) { return this->samples[idx]; }
	sample_t const & operator [] (size_t idx) const { return this->samples[idx]; }
};

#ifndef AUDIO2TEXTURE_HPP
#define AUDIO2TEXTURE_HPP

#include "audionode.hpp"

#include <kiss_fft.h>
#include <kiss_fftr.h>
#include <mutex>

class Audio2Texture :
        public AudioNode
{
public:
    WINDOW_PREAMBLE
private:
    GLuint texture, accuTexture, smoothedTexture;
    Sink * input;
    std::mutex ringbuffer_lock;
    std::vector<AudioStream> ringbuffer;
    std::vector<kiss_fft_cpx> fft_outbuffer;
    std::vector<float> fft_buffer;
    std::vector<float> fft_smoothbuffer;
    std::vector<float> fft_accubuffer;
    kiss_fftr_cfg fftcfg;
    int smoothing;
protected:
	void OnUpdate() override;

	void OnRenderAudio() override;
public:
	Audio2Texture();
	~Audio2Texture() override;

    nlohmann::json Serialize() const override;

	void Deserialize(nlohmann::json const & value) override;
};

#endif // AUDIO2TEXTURE_HPP

#ifndef AUDIO2TEXTURE_HPP
#define AUDIO2TEXTURE_HPP

#include "audionode.hpp"

#include <kiss_fft.h>
#include <kiss_fftr.h>

class Audio2Texture :
        public AudioNode
{
public:
    WINDOW_PREAMBLE
private:
    GLuint texture, accuTexture;
    Sink * input;
	AudioStream data;
    std::vector<kiss_fft_cpx> fft_outbuffer;
    std::vector<float> fft_buffer;
    std::vector<float> fft_accubuffer;
    kiss_fftr_cfg fftcfg;
protected:
	void OnUpdate() override;

	void OnRenderAudio() override;
public:
	Audio2Texture();
	~Audio2Texture() override;
};

#endif // AUDIO2TEXTURE_HPP

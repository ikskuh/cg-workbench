#include "audio2texture.hpp"

#include <kiss_fftr.h>

#include "time.hpp"

#include "windowregistry.hpp"
REGISTER_WINDOW_CLASS(Audio2Texture, Menu::Audio, "audio-fft-transform", "FFT")

Audio2Texture::Audio2Texture() :
    AudioNode("FFT", ImGuiWindowFlags_AlwaysAutoResize),
    texture(0),
    data(1)
{
    this->input = this->AddSink<CgDataType::Audio>("Source");

	this->AddSource<CgDataType::Texture2D>("Default", &this->texture);
    this->AddSource<CgDataType::Texture2D>("Summed", &this->accuTexture);

    this->fft_buffer.resize(audio_buffersize/2 + 1);
    this->fft_outbuffer.resize(this->fft_buffer.size());
    this->fft_accubuffer.resize(this->fft_buffer.size());

    glCreateTextures(GL_TEXTURE_2D, 1, &this->texture);
	glTextureStorage2D(
		this->texture,
		1,
		GL_R32F,
		this->fft_buffer.size() / 2,
		1);
    glTextureParameteri(this->texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(this->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(this->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glCreateTextures(GL_TEXTURE_2D, 1, &this->accuTexture);
	glTextureStorage2D(
		this->accuTexture,
		1,
		GL_R32F,
		this->fft_buffer.size() / 2,
		1);
    glTextureParameteri(this->accuTexture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(this->accuTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(this->accuTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for(float & f : this->fft_accubuffer) {
        f = 0.0f;
    }

    fftcfg = kiss_fftr_alloc( audio_buffersize / 2, false, NULL, NULL );
}

Audio2Texture::~Audio2Texture()
{
    kiss_fft_free( fftcfg );

    if(this->texture != 0)
		glDeleteTextures(1, &this->texture);

    if(this->accuTexture != 0)
		glDeleteTextures(1, &this->accuTexture);
}

void Audio2Texture::OnRenderAudio()
{
	AudioStream const & stream = this->input->GetObject<CgDataType::Audio>();
	stream.CopyTo(this->data);
}

void Audio2Texture::OnUpdate()
{
    kiss_fftr( fftcfg, this->data.GetData(), this->fft_outbuffer.data());

    if(ImGui::Button("Reset Sum"))
    {
        for(float & v : this->fft_accubuffer) {
            v = 0.0f;
        }
    }

    float const scaling = 1.0f / float(audio_buffersize/2);
    for ( size_t i = 0; i < audio_buffersize/2; i++ )
    {
        auto const src = this->fft_outbuffer[ i ];
        const float f = 2.0f * sqrtf( src .r * src .r + src.i * src .i ) * scaling;;

        this->fft_buffer[i] = f;
        this->fft_accubuffer[i] += f * Time::getVideoDelta();
    }

    glTextureSubImage2D(
        this->texture,
        0,
        0, 0,
        this->fft_buffer.size()/2, 1,
        GL_RED,
        GL_FLOAT,
        this->fft_buffer.data());

    glTextureSubImage2D(
        this->accuTexture,
        0,
        0, 0,
        this->fft_accubuffer.size()/2, 1,
        GL_RED,
        GL_FLOAT,
        this->fft_accubuffer.data());
}

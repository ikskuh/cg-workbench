#include "audio2texture.hpp"

#include <kiss_fftr.h>

#include "time.hpp"

#include "windowregistry.hpp"
REGISTER_WINDOW_CLASS(Audio2Texture, Menu::Audio, "audio-fft-transform", "FFT")

static GLuint createTexture(size_t width)
{
    GLuint tex;

    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(
		tex,
		1,
		GL_R32F,
		width,
		1);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return tex;
}

Audio2Texture::Audio2Texture() :
    AudioNode("FFT", ImGuiWindowFlags_AlwaysAutoResize),
    texture(0),
    smoothing(1)
{
    this->input = this->AddSink<CgDataType::Audio>("Source");

    this->AddSource<CgDataType::Texture2D>("Default", &this->texture);
    this->AddSource<CgDataType::Texture2D>("Smoothed", &this->smoothedTexture);
    this->AddSource<CgDataType::Texture2D>("Summed", &this->accuTexture);

    this->fft_buffer.resize(2 * audio_buffersize + 1);
    this->fft_outbuffer.resize(this->fft_buffer.size());
    this->fft_accubuffer.resize(this->fft_buffer.size());
    this->fft_smoothbuffer.resize(this->fft_buffer.size());

    this->texture = createTexture(this->fft_buffer.size() / 4);
    this->accuTexture = createTexture(this->fft_buffer.size() / 4);
    this->smoothedTexture = createTexture(this->fft_buffer.size() / 4);

    for(float & f : this->fft_accubuffer) {
        f = 0.0f;
    }

    fftcfg = kiss_fftr_alloc( 2 * audio_buffersize, false, NULL, NULL );
}

Audio2Texture::~Audio2Texture()
{
    kiss_fft_free( fftcfg );

    if(this->texture != 0)
		glDeleteTextures(1, &this->texture);

    if(this->accuTexture != 0)
		glDeleteTextures(1, &this->accuTexture);

    if(this->smoothedTexture != 0)
		glDeleteTextures(1, &this->smoothedTexture);
}

void Audio2Texture::OnRenderAudio()
{
	AudioStream const & stream = this->input->GetObject<CgDataType::Audio>();

    AudioStream dest = stream;

    std::lock_guard lock { this->ringbuffer_lock };
    if(this->ringbuffer.size() >= 4) { // remove excess from the list
        this->ringbuffer.erase(this->ringbuffer.begin());
    }
    this->ringbuffer.emplace_back(std::move(dest));
}

void Audio2Texture::OnUpdate()
{
    std::vector<float> stream;
    stream.resize(4 * audio_buffersize);
    {
        std::lock_guard lock { this->ringbuffer_lock };
        for(size_t i = 0; i < this->ringbuffer.size(); i++)
        {
            std::copy(
                this->ringbuffer[i].GetData(),
                this->ringbuffer[i].GetData() + audio_buffersize,
                stream.begin() + ssize_t(i)
            );
        }
    }

    kiss_fftr( fftcfg, stream.data(), this->fft_outbuffer.data());

    if(ImGui::Button("Reset Sum"))
    {
        for(float & v : this->fft_accubuffer) {
            v = 0.0f;
        }
    }

    if(ImGui::RadioButton("Smooth", smoothing == 2)) smoothing = 2;
    if(ImGui::RadioButton("Medium", smoothing == 1)) smoothing = 1;
    if(ImGui::RadioButton("Hard",   smoothing == 0)) smoothing = 0;

    float smoothingFactor = 0.4f;
    switch(smoothing) {
    case 0: smoothingFactor = 0.8f; break;
    case 1: smoothingFactor = 0.5f; break;
    case 2: smoothingFactor = 0.3f; break;
    }

    float const scaling = 1.0f / float(audio_buffersize/2);
    for ( size_t i = 0; i < audio_buffersize/2; i++ )
    {
        auto const src = this->fft_outbuffer[ i ];
        const float f = 2.0f * sqrtf( src .r * src .r + src.i * src .i ) * scaling;;

        this->fft_buffer[i] = f;
        this->fft_smoothbuffer[i] = glm::mix(
            fft_smoothbuffer[i],
            f,
            smoothingFactor
        );
        this->fft_accubuffer[i] += fft_smoothbuffer[i] * Time::getVideoDelta();
    }

    glTextureSubImage2D(
        this->texture,
        0,
        0, 0,
        this->fft_buffer.size()/4, 1,
        GL_RED,
        GL_FLOAT,
        this->fft_buffer.data());

    glTextureSubImage2D(
        this->accuTexture,
        0,
        0, 0,
        this->fft_accubuffer.size()/4, 1,
        GL_RED,
        GL_FLOAT,
        this->fft_accubuffer.data());

    glTextureSubImage2D(
        this->smoothedTexture,
        0,
        0, 0,
        this->fft_smoothbuffer.size()/4, 1,
        GL_RED,
        GL_FLOAT,
        this->fft_smoothbuffer.data());
}

nlohmann::json Audio2Texture::Serialize() const {
    return {
        { "smoothing", this->smoothing },
    };
}

void Audio2Texture::Deserialize(nlohmann::json const & value) {
    this->smoothing = value.value("smoothing", int(0));
}

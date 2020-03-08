#include "noisetexture.hpp"

#include <stb_perlin.h>

#include "time.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(NoiseTexture, Menu::Noise, "graphic-noise", "Noise Texture")

NoiseTexture::NoiseTexture() :
    Window("Noise Texture", ImGuiWindowFlags_AlwaysAutoResize),
    staticNoise(true),
    imageFrequency(8.0),
    timeFrequency(10.0)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &this->texture);

	glTextureStorage2D(
		this->texture,
		1,
		GL_RGBA8,
		256,
		256);

	this->AddSource<CgDataType::Texture2D>("2D", &this->texture);

	this->Generate();
}

NoiseTexture::~NoiseTexture()
{
	glDeleteTextures(1, &this->texture);
}

static inline uint8_t denormalize(float f)
{
	return uint8_t(glm::clamp(128.0 + 127.0 * f,0.0,255.0));
}

void NoiseTexture::Generate()
{
	uint8_t noise[4 * 256 * 256];
	for(int x = 0; x < 256; x++)
	{
		for(int y = 0; y < 256; y++)
		{
			uint8_t * const color = &noise[4 * (256 * x + y)];
			color[0] = denormalize(stb_perlin_noise3(
				x / (256.0 / this->imageFrequency),
				y / (256.0 / this->imageFrequency),
				this->timeFrequency * Time::get(),
				256,
				256,
				0));

			color[1] = 0;
			/*
			color[1] = denormalize(stb_perlin_turbulence_noise3(
				x / (256.0 / this->imageFrequency),
				y / (256.0 / this->imageFrequency),
				this->timeFrequency * Time::get(),
				1.0,
				1.0,
				8,
				256,
				256,
				0));
			*/

			color[2] = rand() % 256;
			color[3] = 255;
		}
	}

	glTextureSubImage2D(
		this->texture,
		0,
		0, 0,
		256, 256,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		noise);
}

void NoiseTexture::OnRender()
{
	if(!this->staticNoise)
		this->Generate();
}

void NoiseTexture::OnUpdate()
{
	bool c = false;
	ImGui::Checkbox(
		"Static Noise",
		&this->staticNoise);
	c |= ImGui::DragFloat(
		"Image Frequency",
		&this->imageFrequency,
		0.1f,
		1.0f,
		256.0f,
		"%.0f",
		2.0f);
	c |= ImGui::DragFloat(
		"Time Frequency",
		&this->timeFrequency,
		0.01,
		0.0,
		FLT_MAX);
	if(c)
		this->Generate();
}

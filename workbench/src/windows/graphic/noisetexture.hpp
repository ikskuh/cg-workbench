#pragma once

#include "window.hpp"

class NoiseTexture :
	public Window
{
	WINDOW_PREAMBLE
private:
	GLuint texture;
	bool staticNoise;
	float imageFrequency;
	float timeFrequency;

	void Generate();
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	NoiseTexture();
	~NoiseTexture();
};

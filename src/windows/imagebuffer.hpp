#pragma once

#include "../window.hpp"

class ImageBuffer :
	public Window
{
private:
	GLuint fbo;
	GLuint img;
	GLuint last;
	Sink * input;
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	ImageBuffer();
	~ImageBuffer();

	TRIVIAL_SERIALIZE("image-buffer")
};

#pragma once

#include "window.hpp"
#include "textureeditor.hpp"

class ImageBuffer :
	public Window
{
	WINDOW_PREAMBLE
private:
	GLuint fbo;
	GLuint img;
	GLuint last;
	Sink * input;

	TextureEditor imgEditor;
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	ImageBuffer();
	~ImageBuffer();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

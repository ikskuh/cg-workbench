#pragma once

#include "window.hpp"
#include "textureeditor.hpp"

class ImageSource :
	public Window
{
	WINDOW_PREAMBLE
private:
	GLuint img;

	TextureEditor imgSettings;

	std::string currentFile;
    std::vector<std::byte> currentBlob;

	void SetTexture(GLuint tex);
protected:
	void OnUpdate() override;
public:
	ImageSource();
	~ImageSource();

	void LoadFile();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

#pragma once

#include "../window.hpp"

class ImageSource :
	public Window
{
private:
	GLuint img;
	GLenum minFilter, magFilter;
	GLenum wrapR, wrapS, wrapT;

	std::string currentFile;

	void SetTexture(GLuint tex);
protected:
	void OnUpdate() override;
public:
	ImageSource();
	~ImageSource();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

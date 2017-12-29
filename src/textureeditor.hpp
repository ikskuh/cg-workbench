#pragma once

#include <imgui.h>
#include <GL/gl3w.h>
#include <json.hpp>

class TextureEditor
{
private:
	GLuint img;
public:
	GLenum MinFilter, MagFilter;
	GLenum WrapR, WrapS, WrapT;
public:
	TextureEditor();

	void SetTexture(GLuint tex);

	void Show();

	nlohmann::json Serialize() const;

	void Deserialize(nlohmann::json const & value);
};

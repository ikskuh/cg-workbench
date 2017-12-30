#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include "window.hpp"
#include "textureeditor.hpp"

class RenderWindow :
	public Window
{
private:
	GLuint fb;
	GLuint tex0;
	GLuint tex1;
	GLuint tex2;
	GLuint tex3;
	ImVec2 texSize;
	GLuint depthbuf;
	int editsize[2];

	int scale;
	bool wireframe;
	bool depthtest;

	Sink * geom;
	Sink * shader;
	Sink * transform;

	float mousePressed;
	glm::vec2 mousePos, mousePosNormalized;

	TextureEditor rt0Settings;
	TextureEditor rt1Settings;
	TextureEditor rt2Settings;
	TextureEditor rt3Settings;

	int shownTexture;

	static void SizeConstraint(ImGuiSizeConstraintCallbackData * data);

	void Export(GLuint tex);

	void Regen(GLuint & tex, GLenum format);

	void FmtEdit(GLuint & tex);

	void Resize(int w, int h);
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	RenderWindow();
	RenderWindow(RenderWindow const &) = delete;
	RenderWindow(RenderWindow &&) = delete;
	~RenderWindow();

	ImVec2 GetTextureSize() const { return this->texSize; }

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

#endif // RENDERWINDOW_HPP

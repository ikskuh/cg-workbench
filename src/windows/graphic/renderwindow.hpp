#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include "window.hpp"
#include "textureeditor.hpp"

class RenderWindow :
	public Window
{
private:
	GLuint fb;
	GLuint tex[4];
	ImVec2 texSize;
	GLuint depthbuf;
	int editsize[2];

	int scale;
	bool wireframe;
	bool depthtest;

	bool doClearDepth, doClearColor[4];
	float clearDepth;
	ImVec4 clearColor[4];

	Sink * geom;
	Sink * shader;
	Sink * transform;

	float mousePressed;
	glm::vec2 mousePos, mousePosNormalized;

	TextureEditor rtSettings[4];

	int shownTexture;

	static void SizeConstraint(ImGuiSizeConstraintCallbackData * data);

	void Export(GLuint tex);

	void Regen(int idx, GLenum format);

	void FmtEdit(int idx);

	void ClearEdit(int idx);

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

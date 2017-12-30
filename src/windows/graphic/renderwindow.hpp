#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include "window.hpp"
#include "textureeditor.hpp"

class RenderWindow :
	public Window
{
private:
	GLuint fb;
	GLuint tex;
	ImVec2 texSize;
	GLuint depthbuf;

	int scale;
	bool wireframe;
	bool depthtest;

	Sink * geom;
	Sink * shader;
	Sink * transform;

	float mousePressed;
	glm::vec2 mousePos, mousePosNormalized;

	TextureEditor rt0Settings;

	static void SizeConstraint(ImGuiSizeConstraintCallbackData * data);
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

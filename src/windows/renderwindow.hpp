#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include "../window.hpp"

class RenderWindow :
	public Window
{
private:
	GLuint fb;
	GLuint tex;
	ImVec2 texSize;

	int scale;

	Sink * geom;
	Sink * shader;

	static void SizeConstraint(ImGuiSizeConstraintCallbackData * data);
protected:
	void OnRender() override;
	void OnSetup() override;
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

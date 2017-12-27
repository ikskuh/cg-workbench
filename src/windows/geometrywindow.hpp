#pragma once

#include "../window.hpp"

class GeometryWindow :
	public Window
{
private:
	GLuint vao;
	GLuint vertexBuffer;
protected:
	void OnUpdate() override;
public:
	GeometryWindow();
	GeometryWindow(GeometryWindow const &) = delete;
	GeometryWindow(GeometryWindow &&) = delete;
	~GeometryWindow();

	nlohmann::json Serialize() const override;
};

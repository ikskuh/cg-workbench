#pragma once

#include "geometry.hpp"
#include "window.hpp"

class GeometryWindow :
	public Window
{
	WINDOW_PREAMBLE
private:
	Geometry geom;
	GLuint vertexBuffer;
	std::vector<Vertex> vertices;
	std::string currentFile;
private:
	void Reload();
	void LoadQuad();
	void LoadObj(std::string const & fileName);
protected:
	void OnUpdate() override;
public:
	GeometryWindow();
	GeometryWindow(GeometryWindow const &) = delete;
	GeometryWindow(GeometryWindow &&) = delete;
	~GeometryWindow() override;

	void LoadFile();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

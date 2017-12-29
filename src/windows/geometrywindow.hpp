#pragma once

#include "../geometry.hpp"
#include "../window.hpp"

class GeometryWindow :
	public Window
{
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
	~GeometryWindow();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value);
};

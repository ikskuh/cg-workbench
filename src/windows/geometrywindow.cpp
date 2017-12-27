#include "geometrywindow.hpp"

GeometryWindow::GeometryWindow() :
    Window("Geometry")
{
	glCreateVertexArrays(1, &this->vao);

	glCreateBuffers(1, &this->vertexBuffer);

	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,

	     1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
	};
	glNamedBufferData(
		this->vertexBuffer,
		sizeof(vertices),
	    vertices,
		GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(this->vao, 0);

	glVertexArrayAttribFormat(
		this->vao,
		0, // attrib
		3,
		GL_FLOAT,
		GL_FALSE,
		0);

	glVertexArrayVertexBuffer(
		this->vao,
		0,                  // binding index
		this->vertexBuffer,
		0,                  // offset
		sizeof(float) * 3); // stride

	glVertexArrayAttribBinding(
		this->vao,
		0,  // attrib index
		0); // binding index

	this->AddSource(new Source(CgDataType::Geometry, "Shape", &this->vao));
}

GeometryWindow::~GeometryWindow()
{

}

void GeometryWindow::OnUpdate()
{
	ImGui::Text("Gives triangles to the poor!");
}

nlohmann::json GeometryWindow::Serialize() const
{
	return {
		{ "type", "geometry" },
	};
}

#include "source.hpp"
#include <stdexcept>

Source::Source(CgDataType type, std::string const & name, void const * object) :
	Slot(type, name),
    object(object)
{
	if(this->object == nullptr)
		throw std::invalid_argument("object must not be null.");
}

Source::~Source()
{

}



static Source * GetGeometryDefault()
{
	static Source * src = nullptr;
	if(src != nullptr)
		return src;

	static GLuint vao, vertexBuffer;

	glCreateVertexArrays(1, &vao);

	glCreateBuffers(1, &vertexBuffer);

	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,

	     1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
	};
	glNamedBufferData(
		vertexBuffer,
		sizeof(vertices),
	    vertices,
		GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(vao, 0);

	glVertexArrayAttribFormat(
		vao,
		0, // attrib
		3,
		GL_FLOAT,
		GL_FALSE,
		0);

	glVertexArrayVertexBuffer(
		vao,
		0,                  // binding index
		vertexBuffer,
		0,                  // offset
		sizeof(float) * 3); // stride

	glVertexArrayAttribBinding(
		vao,
		0,  // attrib index
		0); // binding index

	src = new Source(CgDataType::Geometry, "Shape", &vao);
	return src;
}

Source const * Source::GetDefault(CgDataType type)
{
	switch(type)
	{
		case CgDataType::Geometry: return GetGeometryDefault();
		default: return nullptr;
	}
}

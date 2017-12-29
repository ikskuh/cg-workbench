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

#define A 0
#define B 1
	Vertex vertices[] = {
	    Vertex {
			position: { -1.0f, -1.0f, 0.0f },
			normal: { 0,0,1 },
			uv: {0,A}
		},
		Vertex {
			position: {1.0f, -1.0f, 0.0f},
			normal: {0,0,1},
			uv: {1,A}
		},
		Vertex {
			position: {-1.0f,  1.0f, 0.0f},
			normal: {0,0,1},
			uv: {0,B}
		},
		Vertex {
			position: {1.0f,  1.0f, 0.0f},
			normal: {0,0,1},
			uv: {1,B}
		},
		Vertex {
			position: {1.0f, -1.0f, 0.0f},
			normal: {0,0,1},
			uv: {1,A}
		},
		Vertex {
			position: {-1.0f,  1.0f, 0.0f},
			normal: {0,0,1},
			uv: {0,B}
		},
	};
	glNamedBufferData(
		vertexBuffer,
		6 * sizeof(Vertex),
	    vertices,
		GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);

	glVertexArrayAttribFormat(
		vao,
		0, // attrib
		3,
		GL_FLOAT,
		GL_FALSE,
		offsetof(Vertex,position));
	glVertexArrayAttribFormat(
		vao,
		1, // attrib
		3,
		GL_FLOAT,
		GL_FALSE,
		offsetof(Vertex,normal));
	glVertexArrayAttribFormat(
		vao,
		2, // attrib
		2,
		GL_FLOAT,
		GL_FALSE,
		offsetof(Vertex,uv));

	glVertexArrayVertexBuffer(
		vao,
		0,                  // binding index
		vertexBuffer,
		0,                  // offset
		sizeof(Vertex)); // stride

	glVertexArrayAttribBinding(
		vao,
		0,  // attrib index
		0); // binding index
	glVertexArrayAttribBinding(
		vao,
		1,  // attrib index
		0); // binding index
	glVertexArrayAttribBinding(
		vao,
		2,  // attrib index
		0); // binding index

	static Geometry geom;
	geom.VertexArray = vao;
	geom.VertexCount = 6;

	src = new Source(CgDataType::Geometry, "Shape", &geom);
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

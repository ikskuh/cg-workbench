#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>

struct Vertex
{
	float position[4];
	float normal[4];
	float uv[4];
} __attribute__((packed));

struct Geometry
{
	int VertexCount;
	GLuint VertexArray;
};

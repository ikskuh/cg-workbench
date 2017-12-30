#pragma once

#include <GL/gl3w.h>
#include <string>

namespace resources
{
	void load(std::string const & root);

	namespace icons
	{
		extern GLuint geometry;
		extern GLuint shader;
		extern GLuint image;
		extern GLuint genericSink;
		extern GLuint genericSource;
		extern GLuint matrix;
		extern GLuint scalar;
		extern GLuint vector2;
		extern GLuint vector3;
		extern GLuint vector4;
	}
}

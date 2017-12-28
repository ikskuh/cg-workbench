#include "resources.hpp"
#include "imageloader.hpp"
#include <assert.h>

namespace resources
{
	void load(std::string const & root)
	{
	#define ICON(_name, file) \
		icons::_name = load_texture(root + "/icons/" file, GL_CLAMP_TO_BORDER); \
		assert(icons::_name != 0)
		ICON(geometry, "geometry.png");
		ICON(shader, "shader.png");
		ICON(image, "image.png");
		ICON(genericSink, "power-socket.png");
		ICON(genericSource, "power-plug.png");
		ICON(scalar, "numeric.png");
		ICON(vector, "numeric.png");
		ICON(matrix, "matrix.png");
	}

	namespace icons
	{
		GLuint geometry;
		GLuint shader;
		GLuint image;
		GLuint genericSink;
		GLuint genericSource;
		GLuint matrix;
		GLuint scalar;
		GLuint vector;
	}
}

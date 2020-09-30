#include "resources.hpp"
#include "imageloader.hpp"
#include <assert.h>

extern "C"
{

	struct Resource
	{
		char const *file_name;
		uint8_t const *data;
		size_t length;
	};

	extern int cgw_find_resource(char const *name, Resource *resource);
};

namespace resources
{
	void load(std::string const &root)
	{
		// TODO: https://github.com/MasterQ32/cg-workbench/issues/3
#define ICON(_name, file)                                                              \
	{                                                                                    \
		Resource res;                                                                      \
		if (!cgw_find_resource("/icons/" file, &res))                                      \
			abort();                                                                         \
		icons::_name = load_texture_from_memory(res.data, res.length, GL_CLAMP_TO_BORDER); \
		assert(icons::_name != 0);                                                         \
	}
		ICON(geometry, "geometry.png");
		ICON(shader, "shader.png");
		ICON(image, "image.png");
		ICON(genericSink, "power-socket.png");
		ICON(genericSource, "power-plug.png");
		ICON(scalar, "numeric.png");
		ICON(vector2, "vec2.png");
		ICON(vector3, "vec3.png");
		ICON(vector4, "vec4.png");
		ICON(matrix, "matrix.png");
		ICON(audio, "audio.png");
		ICON(event_source, "event_source.png");
		ICON(event_listener, "event_listener.png");
		ICON(render_pass, "render_pass.png");
	}

	namespace icons
	{
		GLuint geometry;
		GLuint shader;
		GLuint image;
		GLuint audio;
		GLuint render_pass;
		GLuint event_source;
		GLuint event_listener;
		GLuint genericSink;
		GLuint genericSource;
		GLuint matrix;
		GLuint scalar;
		GLuint vector2;
		GLuint vector3;
		GLuint vector4;
	} // namespace icons
} // namespace resources

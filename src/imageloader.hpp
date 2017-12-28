#pragma once

#include <GL/gl3w.h>
#include <string>

GLuint load_texture(std::string const & fileName, GLenum wrap = GL_REPEAT, bool withMipMaps = true);

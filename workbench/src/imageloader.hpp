#pragma once

#include <GL/gl3w.h>
#include <string>

GLuint load_texture(std::string const &fileName, GLenum wrap = GL_REPEAT, bool withMipMaps = true);

GLuint load_texture_from_memory(uint8_t const *buffer, size_t length, GLenum wrap = GL_REPEAT, bool withMipMaps = true);

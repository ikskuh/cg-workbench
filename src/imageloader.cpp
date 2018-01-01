#include "imageloader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <cmath>

GLuint load_texture(std::string const & fileName, GLenum wrap, bool withMipMaps)
{
	int w, h, chan;
	auto * ptr = stbi_load(fileName.c_str(), &w, &h, &chan, 4);
	if(!ptr)
		return 0;

	GLuint img;
	glCreateTextures(GL_TEXTURE_2D, 1, &img);

	int levels = 1;
	if(withMipMaps)
        levels = std::ceil(std::log2(std::max(w,h)));

	glTextureStorage2D(
		img,
		levels,
		GL_RGBA8,
		w, h);

	glTextureSubImage2D(
		img,
		0,
		0, 0,
		w, h,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		ptr);

	if(withMipMaps)
		glGenerateTextureMipmap(img);

	if(withMipMaps)
		glTextureParameteri(img, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	else
		glTextureParameteri(img, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(img, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(img, GL_TEXTURE_WRAP_R, wrap);
	glTextureParameteri(img, GL_TEXTURE_WRAP_S, wrap);

	return img;
}

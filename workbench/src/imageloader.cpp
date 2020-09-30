#include "imageloader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <cmath>
#include <vector>

static GLuint load_raw(stbi_uc *ptr, int w, int h, GLenum wrap, bool withMipMaps)
{
	// (╯°□°）╯︵ ┻━┻
	{
		size_t stride = w * 4;
		std::vector<stbi_uc> scanline(stride);
		for (int y = 0; y < h / 2; y++) // rounding error is good:
		{
			stbi_uc *top = ptr + y * stride;
			stbi_uc *bot = ptr + (h - y - 1) * stride;

			memcpy(scanline.data(), top, stride);
			memcpy(top, bot, stride);
			memcpy(bot, scanline.data(), stride);
		}
	}

	GLuint img;
	glCreateTextures(GL_TEXTURE_2D, 1, &img);

	int levels = 1;
	if (withMipMaps)
		levels = std::ceil(std::log2(std::max(w, h)));

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

	if (withMipMaps)
		glGenerateTextureMipmap(img);

	if (withMipMaps)
		glTextureParameteri(img, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	else
		glTextureParameteri(img, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(img, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(img, GL_TEXTURE_WRAP_R, wrap);
	glTextureParameteri(img, GL_TEXTURE_WRAP_S, wrap);

	stbi_image_free(ptr);

	return img;
}

GLuint load_texture(std::string const &fileName, GLenum wrap, bool withMipMaps)
{
	int w, h, chan;
	stbi_uc *ptr = stbi_load(fileName.c_str(), &w, &h, &chan, 4);
	return load_raw(ptr, w, h, wrap, withMipMaps);
}

GLuint load_texture_from_memory(uint8_t const *buffer, size_t length, GLenum wrap, bool withMipMaps)
{
	int w, h, chan;
	stbi_uc *ptr = stbi_load_from_memory(buffer, length, &w, &h, &chan, 4);
	return load_raw(ptr, w, h, wrap, withMipMaps);
}

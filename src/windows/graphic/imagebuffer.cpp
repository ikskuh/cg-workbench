#include "imagebuffer.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(ImageBuffer, Menu::Buffer, "image-buffer", "Image Buffer");

ImageBuffer::ImageBuffer() :
    Window("Image Buffer", ImGuiWindowFlags_AlwaysAutoResize),
    img(0),
    last(0)
{
	this->AddSink(this->input = new Sink(CgDataType::Texture2D, "Input"));
	this->AddSource(new Source(CgDataType::Texture2D, "Output", &this->img));

	glCreateFramebuffers(1, &this->fbo);
}

ImageBuffer::~ImageBuffer()
{
	if(this->img != 0)
		glDeleteTextures(1, &this->img);
}

void ImageBuffer::OnRender()
{
	GLuint src = this->input->GetObject<CgDataType::Texture2D>();
	if(this->last != src)
	{
		if(this->img != 0)
			glDeleteTextures(1, &this->img);
		this->img = 0;
		this->last = src;

		if(src != 0)
		{
			int w, h;
			GLenum type;
			glGetTextureLevelParameteriv(src, 0, GL_TEXTURE_WIDTH, &w);
			glGetTextureLevelParameteriv(src, 0, GL_TEXTURE_HEIGHT, &h);
			glGetTextureLevelParameteriv(src, 0, GL_TEXTURE_RED_TYPE, reinterpret_cast<GLint*>(&type));

			int idx = 0;
			switch(type)
			{
				case GL_UNSIGNED_NORMALIZED: idx = 0; break;
				case GL_FLOAT:               idx = 1; break;
				case GL_INT:                 idx = 2; break;
				case GL_UNSIGNED_INT:        idx = 3; break;
				default:                     idx = 0; break;
			}
			static GLenum formats[] = {
			    GL_RGBA8, GL_RGBA32F, GL_RGBA8I, GL_RGBA8UI
			};

			glCreateTextures(GL_TEXTURE_2D, 1, &this->img);
			glTextureStorage2D(
				this->img,
				1,
				formats[idx],
				w, h);

			glNamedFramebufferTexture(
				this->fbo,
				GL_COLOR_ATTACHMENT0,
			    this->img,
				0);
		}

		this->imgEditor.SetTexture(this->img);
	}

	if((src == 0) && (this->img == 0))
		return;

	glNamedFramebufferTexture(
		this->fbo,
		GL_COLOR_ATTACHMENT0,
	    src,
		0);

	int w, h;
	glGetTextureLevelParameteriv(this->img, 0, GL_TEXTURE_WIDTH, &w);
	glGetTextureLevelParameteriv(this->img, 0, GL_TEXTURE_HEIGHT, &h);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
	glCopyTextureSubImage2D(
		this->img,
		0,
		0, 0,
		0, 0,
		w, h);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

}

void ImageBuffer::OnUpdate()
{
	ImGui::Text("Buffers a texturefor one frame.");

	if(this->img != 0)
		this->imgEditor.Show();
}



nlohmann::json ImageBuffer::Serialize() const
{
	return {
		{ "type", ImageBufferID },
		{ "filter", this->imgEditor.Serialize() }
	};
}

void ImageBuffer::Deserialize(const nlohmann::json &value)
{
	try {
		this->imgEditor.Deserialize(value.at("filter"));
	} catch(nlohmann::detail::out_of_range const & ex) {
		// meh.
	}
}

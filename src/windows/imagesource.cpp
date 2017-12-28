#include "imagesource.hpp"
#include <nfd.h>
#include <unistd.h>
#include "../imageloader.hpp"

static GLenum const minFilters[] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_LINEAR
};
static char const * const minFilterNames[] =
{
	"Nearest",
	"Linear",
    "Nearest (Mipmap Nearest)",
    "Nearest (Mipmap Linear)",
    "Linear (Mipmap Nearest)",
    "Linear (Mipmap Linear)",
};


static GLenum const wrapModes[] =
{
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
    GL_MIRROR_CLAMP_TO_EDGE,
};
static char const * const wrapModeNames[] =
{
    "Repeat",
    "Mirrored Repeat",
    "Clamp To Edge",
    "Mirror Clamp To Edge"
};


static int GetCurrent(GLenum const * items, int cnt, GLenum current)
{
	for(int i = 0; i < cnt; i++)
	{
		if(items[i] == current)
			return i;
	}
	return 0;
}

ImageSource::ImageSource() :
    Window("Image", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar),
    img(0),
    currentFile("")
{
	this->AddSource(new Source(CgDataType::Texture2D, "Texture", &this->img));
}

ImageSource::~ImageSource()
{
	if(this->img != 0)
		glDeleteTextures(1, &this->img);
}

void ImageSource::OnUpdate()
{
	char cwd[256];
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Image"))
		{
			if(ImGui::MenuItem("Load..."))
			{
				nfdchar_t *outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("png,jpeg,jpg,bmp", getcwd(cwd, sizeof(cwd)), &outPath );
				if ( result == NFD_OKAY )
				{
					std::string fileName(outPath);
					free(outPath);

					GLuint i = load_texture(fileName);
					if(i != 0) {
						this->SetTexture(i);
						this->currentFile = fileName;
					}
				}
				else if ( result == NFD_CANCEL )
					; // Silently ignore cancel
				else
				{
					printf("Error: %s\n", NFD_GetError() );
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if(this->img != 0)
	{
		int w, h;
		glGetTextureLevelParameteriv(this->img, 0, GL_TEXTURE_WIDTH, &w);
		glGetTextureLevelParameteriv(this->img, 0, GL_TEXTURE_HEIGHT, &h);

		ImGui::Image(
			ImTextureID(uintptr_t(this->img)),
			ImVec2(256, 256 * h / w));

		int idx;

#define PARAM(_Label, _Src, _Names, _Cnt, _Var, _Param) \
		idx = GetCurrent(_Src, _Cnt, this->_Var); \
		ImGui::Combo( \
			_Label, \
			&idx, \
			_Names, \
			_Cnt); \
		if(_Src[idx] != this->_Var) \
			glTextureParameteri(this->img, _Param, this->_Var = _Src[idx])

		PARAM("Min Filter", minFilters, minFilterNames, 6, minFilter, GL_TEXTURE_MIN_FILTER);
		PARAM("Mag Filter", minFilters, minFilterNames, 2, magFilter, GL_TEXTURE_MAG_FILTER);

		PARAM("Wrap R", wrapModes, wrapModeNames, 4, wrapR, GL_TEXTURE_WRAP_R);
		PARAM("Wrap S", wrapModes, wrapModeNames, 4, wrapS, GL_TEXTURE_WRAP_S);
		PARAM("Wrap T", wrapModes, wrapModeNames, 4, wrapT, GL_TEXTURE_WRAP_T);

	}
	else
	{
		ImGui::Text("No image loaded");
	}
}

void ImageSource::SetTexture(GLuint tex)
{
	if(this->img == tex)
		return;
	if(this->img != 0)
		glDeleteTextures(1, &this->img);
	this->img = tex;

	glGetTextureParameteriv(this->img, GL_TEXTURE_MIN_FILTER, reinterpret_cast<GLint*>(&this->minFilter));
	glGetTextureParameteriv(this->img, GL_TEXTURE_MAG_FILTER, reinterpret_cast<GLint*>(&this->magFilter));
	glGetTextureParameteriv(this->img, GL_TEXTURE_WRAP_R, reinterpret_cast<GLint*>(&this->wrapR));
	glGetTextureParameteriv(this->img, GL_TEXTURE_WRAP_S, reinterpret_cast<GLint*>(&this->wrapS));
	glGetTextureParameteriv(this->img, GL_TEXTURE_WRAP_T, reinterpret_cast<GLint*>(&this->wrapT));
}



nlohmann::json ImageSource::Serialize() const
{
	return {
		{ "type", "image" },
		{ "wrap-r", this->wrapR },
		{ "wrap-s", this->wrapS },
		{ "wrap-t", this->wrapT },
		{ "min-filt", this->minFilter },
		{ "mag-filt", this->magFilter },
		{ "file", this->currentFile },
	};
}

void ImageSource::Deserialize(const nlohmann::json &value)
{
	this->wrapR = GLenum(int(value["wrap-r"]));
	this->wrapS = GLenum(int(value["wrap-s"]));
	this->wrapT = GLenum(int(value["wrap-t"]));
	this->minFilter = GLenum(int(value["min-filt"]));
	this->magFilter = GLenum(int(value["mag-filt"]));
	this->currentFile = value["file"];
	if(this->currentFile.length() == 0)
		return;

	GLuint i = load_texture(this->currentFile);
	if(i == 0)
		return;
	this->SetTexture(i);

	glTextureParameteri(this->img, GL_TEXTURE_WRAP_R, this->wrapR);
	glTextureParameteri(this->img, GL_TEXTURE_WRAP_S, this->wrapS);
	glTextureParameteri(this->img, GL_TEXTURE_WRAP_T, this->wrapT);
	glTextureParameteri(this->img, GL_TEXTURE_MIN_FILTER, this->minFilter);
	glTextureParameteri(this->img, GL_TEXTURE_MAG_FILTER, this->magFilter);

}

#include "textureeditor.hpp"

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


TextureEditor::TextureEditor() :
    img(0),
    MinFilter(GL_LINEAR), MagFilter(GL_LINEAR),
    WrapR(GL_REPEAT), WrapS(GL_REPEAT), WrapT(GL_REPEAT)
{

}

void TextureEditor::SetTexture(GLuint tex, bool overwrite)
{
	if(this->img == tex)
		return;
	this->img = tex;
	if(overwrite)
		this->WriteParams();
	else
		this->ReadParams();
}

void TextureEditor::Show()
{
	int idx;

	if(this->img == 0)
	{
		ImGui::Text("No image set.");
		return;
	}

#define PARAM(_Label, _Src, _Names, _Cnt, _Var, _Param) \
	idx = GetCurrent(_Src, _Cnt, this->_Var); \
	ImGui::Combo( \
		_Label, \
		&idx, \
		_Names, \
		_Cnt); \
	if(_Src[idx] != this->_Var) \
		glTextureParameteri(this->img, _Param, this->_Var = _Src[idx])

	PARAM("Min Filter", minFilters, minFilterNames, 6, MinFilter, GL_TEXTURE_MIN_FILTER);
	PARAM("Mag Filter", minFilters, minFilterNames, 2, MagFilter, GL_TEXTURE_MAG_FILTER);

	PARAM("Wrap R", wrapModes, wrapModeNames, 4, WrapR, GL_TEXTURE_WRAP_R);
	PARAM("Wrap S", wrapModes, wrapModeNames, 4, WrapS, GL_TEXTURE_WRAP_S);
	PARAM("Wrap T", wrapModes, wrapModeNames, 4, WrapT, GL_TEXTURE_WRAP_T);

}

nlohmann::json TextureEditor::Serialize() const
{
	return {
		{ "wrap-r", this->WrapR },
		{ "wrap-s", this->WrapS },
		{ "wrap-t", this->WrapT },
		{ "min-filt", this->MinFilter },
		{ "mag-filt", this->MagFilter },
	};
}

void TextureEditor::Deserialize(nlohmann::json const & value)
{
	this->WrapR = value.value("wrap-r", GL_REPEAT);
	this->WrapS = value.value("wrap-s", GL_REPEAT);
	this->WrapT = value.value("wrap-t", GL_REPEAT);
	this->MinFilter = value.value("min-filt", GL_LINEAR);
	this->MagFilter = value.value("mag-filt", GL_LINEAR);

	if(this->img == 0)
		return;

	this->WriteParams();
}


void TextureEditor::ReadParams()
{
	glGetTextureParameteriv(this->img, GL_TEXTURE_MIN_FILTER, reinterpret_cast<GLint*>(&this->MinFilter));
	glGetTextureParameteriv(this->img, GL_TEXTURE_MAG_FILTER, reinterpret_cast<GLint*>(&this->MagFilter));
	glGetTextureParameteriv(this->img, GL_TEXTURE_WRAP_R, reinterpret_cast<GLint*>(&this->WrapR));
	glGetTextureParameteriv(this->img, GL_TEXTURE_WRAP_S, reinterpret_cast<GLint*>(&this->WrapS));
	glGetTextureParameteriv(this->img, GL_TEXTURE_WRAP_T, reinterpret_cast<GLint*>(&this->WrapT));
}

void TextureEditor::WriteParams()
{
	glTextureParameteri(this->img, GL_TEXTURE_WRAP_R, this->WrapR);
	glTextureParameteri(this->img, GL_TEXTURE_WRAP_S, this->WrapS);
	glTextureParameteri(this->img, GL_TEXTURE_WRAP_T, this->WrapT);
	glTextureParameteri(this->img, GL_TEXTURE_MIN_FILTER, this->MinFilter);
	glTextureParameteri(this->img, GL_TEXTURE_MAG_FILTER, this->MagFilter);
}

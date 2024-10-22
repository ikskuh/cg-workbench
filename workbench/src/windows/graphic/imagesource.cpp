#include "imagesource.hpp"
#include "imageloader.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(ImageSource, Menu::Graphic, "image", "Image")

#include "fileio.hpp"

ImageSource::ImageSource() : Window("Image", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar),
														 img(0),
														 currentFile("")
{
	this->AddSource(new Source(CgDataType::Texture2D, "Texture", &this->img));
}

ImageSource::~ImageSource()
{
	if (this->img != 0)
		glDeleteTextures(1, &this->img);
}

void ImageSource::OnUpdate()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Image"))
		{
			if (ImGui::MenuItem("Load..."))
				this->LoadFile();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (this->img != 0)
	{
		int w, h;
		glGetTextureLevelParameteriv(this->img, 0, GL_TEXTURE_WIDTH, &w);
		glGetTextureLevelParameteriv(this->img, 0, GL_TEXTURE_HEIGHT, &h);

		ImGui::Image(
				ImTextureID(uintptr_t(this->img)),
				ImVec2(256, 256 * h / w),
				ImVec2(0, 1),
				ImVec2(1, 0));

		this->imgSettings.Show();
	}
	else
	{
		ImGui::Text("No image loaded");
	}
}

void ImageSource::SetTexture(GLuint tex)
{
	if (this->img == tex)
		return;
	if (this->img != 0)
		glDeleteTextures(1, &this->img);

	this->img = tex;
	imgSettings.SetTexture(tex);
}

nlohmann::json ImageSource::Serialize() const
{
	return {
			{"filter", this->imgSettings.Serialize()},
			{"file", this->currentFile},
	};
}

void ImageSource::Deserialize(const nlohmann::json &value)
{
	this->currentFile = value["file"].get<std::string>();
	if (this->currentFile.length() == 0)
		return;

	GLuint i = load_texture(this->currentFile);
	if (i == 0)
		return;
	this->SetTexture(i);

	try
	{
		this->imgSettings.Deserialize(value.at("filter"));
	}
	catch (nlohmann::detail::out_of_range const &ex)
	{
	}
}

void ImageSource::LoadFile()
{
	auto path = FileIO::OpenDialog("png,jpeg,jpg,bmp");
	GLuint i = load_texture(path);
	if (i != 0)
	{
		this->SetTexture(i);
		this->currentFile = path;
	}
}

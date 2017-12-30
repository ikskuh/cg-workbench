#include "renderwindow.hpp"
#include <nfd.h>
#include <fstream>
#include <unistd.h>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(RenderWindow, Menu, "renderer", "Renderer");

RenderWindow::RenderWindow() :
    Window("Render Window", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize),
    texSize(512,512),
    scale(9), wireframe(false), depthtest(true),
    mousePressed(0.0f), mousePos(256,256)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &this->tex);
	glTextureStorage2D(
		this->tex,
		1,
		GL_RGBA8,
		this->texSize.x,
		this->texSize.y);
	glTextureParameteri(this->tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(this->tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glCreateRenderbuffers(1, &this->depthbuf);
	glNamedRenderbufferStorage(
		this->depthbuf,
		GL_DEPTH24_STENCIL8,
		this->texSize.x,
		this->texSize.y);

	glCreateFramebuffers(1, &this->fb);
	glNamedFramebufferTexture(
		this->fb,
		GL_COLOR_ATTACHMENT0,
	    this->tex,
		0);
	glNamedFramebufferRenderbuffer(
		this->fb,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		this->depthbuf);
	auto status = glCheckNamedFramebufferStatus(this->fb, GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
		throw "foo";

	rt0Settings.SetTexture(this->tex);

	this->AddSink(this->geom = new Sink(CgDataType::Geometry, "Geometry"));
	this->AddSink(this->shader = new Sink(CgDataType::Shader, "Shader"));

	this->AddSource(new Source(CgDataType::Texture2D, "Image", &this->tex));
	this->AddSource(new Source(CgDataType::UniformVec2, "Image Size", &this->texSize.x));
	this->AddSource(new Source(CgDataType::UniformFloat, "Mouse Pressed", &this->mousePressed));
	this->AddSource(new Source(CgDataType::UniformVec2, "Mouse Pos", &this->mousePos.x));
	this->AddSource(new Source(CgDataType::UniformVec2, "Mouse Pos (Normalized)", &this->mousePosNormalized.x));
}

RenderWindow::~RenderWindow()
{
	glDeleteFramebuffers(1, &this->fb);
	glDeleteTextures(1, &this->tex);
	glDeleteRenderbuffers(1, &this->depthbuf);
}

void RenderWindow::OnRender()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fb);

	glViewport(0, 0, this->texSize.x, this->texSize.y);
	glDisable(GL_SCISSOR_TEST);

	if(this->depthtest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if(this->shader->HasSource() && this->geom->HasSource())
	{
		glClearDepth(1.0f);
		glClearColor(0.0,0.0,0.0,1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto const & geom = this->geom->GetObject<CgDataType::Geometry>();

		glUseProgram(this->shader->GetObject<CgDataType::Shader>());
		glBindVertexArray(geom.VertexArray);

		if(this->wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawArrays(GL_TRIANGLES, 0, geom.VertexCount);
	}
	else
	{
		glClearColor(1,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void RenderWindow::SizeConstraint(ImGuiSizeConstraintCallbackData * data)
{
	RenderWindow * window = static_cast<RenderWindow*>(data->UserData);

	int w, h;
	glGetTextureLevelParameteriv(window->tex, 0, GL_TEXTURE_WIDTH, &w);
	glGetTextureLevelParameteriv(window->tex, 0, GL_TEXTURE_HEIGHT, &h);

	float aspect = float(w) / float(h);

	float py = 2.0 * ImGui::GetStyle().WindowPadding.y
		+ 3.0 * ImGui::GetStyle().FramePadding.y
		+ ImGui::GetFont()->FontSize;
	float px = 2.0 * ImGui::GetStyle().WindowPadding.x
		- ImGui::GetStyle().ScrollbarSize;

	ImVec2 src = data->DesiredSize;

	data->DesiredSize = ImVec2(
		src.x,
		(src.x - px) / aspect + py);
}

void RenderWindow::OnUpdate()
{
	char cwd[256];
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Renderer"))
		{
			if(ImGui::BeginMenu("Viewport"))
			{
				bool scale7 = (this->scale == 7);
				bool scale8 = (this->scale == 8);
				bool scale9 = (this->scale == 9);
				bool scale10 = (this->scale == 10);
				ImGui::MenuItem("128 px", nullptr, &scale7);
				ImGui::MenuItem("256 px", nullptr, &scale8);
				ImGui::MenuItem("512 px", nullptr, &scale9);
				ImGui::MenuItem("1024 px", nullptr, &scale10);

	#define APPLY(n) \
				else if((this->scale != n) && scale##n) this->scale = n
				if(false) ;
				APPLY(7);
				APPLY(8);
				APPLY(9);
				APPLY(10);
	#undef APPLY

				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Filter Settings"))
			{
				this->rt0Settings.Show();
				ImGui::EndMenu();
			}

			ImGui::Separator();

			ImGui::MenuItem("Wireframe", nullptr, &this->wireframe);
			ImGui::MenuItem("Depth Test", nullptr, &this->depthtest);

			ImGui::Separator();

			if(ImGui::MenuItem("Export..."))
			{
				nfdchar_t *outPath = NULL;
				nfdresult_t result = NFD_SaveDialog("ppm", getcwd(cwd, sizeof(cwd)), &outPath );
				if ( result == NFD_OKAY )
				{
					std::string fileName(outPath);
					free(outPath);

					int w, h;
					glGetTextureLevelParameteriv(this->tex, 0, GL_TEXTURE_WIDTH, &w);
					glGetTextureLevelParameteriv(this->tex, 0, GL_TEXTURE_HEIGHT, &h);

					std::vector<char> bits(w * h * 3);
					glGetTextureImage(
						this->tex,
						0,
						GL_RGB,
						GL_UNSIGNED_BYTE,
						bits.size(),
						bits.data());

					std::ofstream stream(fileName);
					stream << "P6 " << w << " " << h << " 255" << "\n";
					stream.write(bits.data(), bits.size());
					stream.flush();
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

	int w, h;
	glGetTextureLevelParameteriv(this->tex, 0, GL_TEXTURE_WIDTH, &w);
	glGetTextureLevelParameteriv(this->tex, 0, GL_TEXTURE_HEIGHT, &h);

	float size = (1 << this->scale);

	auto pos = ImGui::GetCursorPos();
	auto vpsize = ImVec2(size, (size / w) * h);
	ImGui::ImageButton(
		ImTextureID(uintptr_t(this->tex)),
	    vpsize,
		ImVec2(0.0f, 0.0f),
		ImVec2(1.0f, 1.0f),
		0);
	if(ImGui::IsItemHovered())
	{
		this->mousePressed = ImGui::IsMouseDown(0);

		this->mousePos = ImGui::GetMousePos();
		this->mousePos -= (glm::vec2)ImGui::GetWindowPos();
		this->mousePos -= (glm::vec2)pos;

		this->mousePosNormalized = this->mousePos / glm::vec2(vpsize.x, vpsize.y);
	}
}

nlohmann::json RenderWindow::Serialize() const
{
	return {
		{ "type", RenderWindowID },
		{ "scale", this->scale },
		{ "wireframe", this->wireframe },
		{ "depthtest", this->depthtest },
		{ "rt0-settings", this->rt0Settings.Serialize() },
	};
}

void RenderWindow::Deserialize(const nlohmann::json &value)
{
	this->scale = value["scale"];
	this->wireframe = value.value("wireframe", false);
	this->depthtest = value.value("depthtest", true);

	try {
		this->rt0Settings.Deserialize(value.at("rt0-settings"));
	} catch(nlohmann::detail::out_of_range const & ex) {

	}
}

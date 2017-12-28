#include "renderwindow.hpp"

RenderWindow::RenderWindow() :
    Window("Render Window", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize),
    texSize(512,512),
    scale(9), wireframe(false)
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

	glCreateFramebuffers(1, &this->fb);
	glNamedFramebufferTexture(
		this->fb,
		GL_COLOR_ATTACHMENT0,
	    this->tex,
		0);
	auto status = glCheckNamedFramebufferStatus(this->fb, GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
		throw "foo";

	this->AddSink(this->geom = new Sink(CgDataType::Geometry, "Geometry"));
	this->AddSink(this->shader = new Sink(CgDataType::Shader, "Shader"));

	this->AddSource(new Source(CgDataType::Texture2D, "Image", &this->tex));
}

RenderWindow::~RenderWindow()
{
	glDeleteFramebuffers(1, &this->fb);
	glDeleteTextures(1, &this->tex);
}

void RenderWindow::OnRender()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fb);

	glViewport(0, 0, this->texSize.x, this->texSize.y);
	glDisable(GL_SCISSOR_TEST);

	glDisable(GL_DEPTH_TEST);
	if(this->shader->HasSource() && this->geom->HasSource())
	{
		glClearColor(0.0,0.0,0.0,1);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(this->shader->GetObject<CgDataType::Shader>());
		glBindVertexArray(this->geom->GetObject<CgDataType::Geometry>());

		if(this->wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawArrays(GL_TRIANGLES, 0, 6);
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
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Renderer"))
		{
			if(ImGui::BeginMenu("Render Targets"))
			{
				if(ImGui::BeginMenu("Target 1"))
				{
					ImGui::MenuItem("None");
					ImGui::MenuItem("RGBA8");
					ImGui::MenuItem("RGBA16F");
					ImGui::MenuItem("RGBA32F");
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu("Target 2"))
				{
					ImGui::MenuItem("None");
					ImGui::MenuItem("RGBA8");
					ImGui::MenuItem("RGBA16F");
					ImGui::MenuItem("RGBA32F");
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu("Target 3"))
				{
					ImGui::MenuItem("None");
					ImGui::MenuItem("RGBA8");
					ImGui::MenuItem("RGBA16F");
					ImGui::MenuItem("RGBA32F");
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu("Target 4"))
				{
					ImGui::MenuItem("None");
					ImGui::MenuItem("RGBA8");
					ImGui::MenuItem("RGBA16F");
					ImGui::MenuItem("RGBA32F");
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

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

			ImGui::MenuItem("Wireframe", nullptr, &this->wireframe);

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	int w, h;
	glGetTextureLevelParameteriv(this->tex, 0, GL_TEXTURE_WIDTH, &w);
	glGetTextureLevelParameteriv(this->tex, 0, GL_TEXTURE_HEIGHT, &h);

	float size = (1 << this->scale);

	ImGui::Image(
		ImTextureID(uintptr_t(this->tex)),
	    ImVec2(size, (size / w) * h));
}

nlohmann::json RenderWindow::Serialize() const
{
	return {
		{ "type", "renderer" },
		{ "scale", this->scale },
		{ "wireframe", this->wireframe },
	};
}

void RenderWindow::Deserialize(const nlohmann::json &value)
{
	this->scale = value["scale"];
	this->wireframe = value["wireframe"];
}

#include "renderwindow.hpp"
#include <fstream>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(RenderWindow, Menu::Graphic, "renderer", "Renderer")

#include "fileio.hpp"

static int gentex(ImVec2 size, GLenum format)
{
	GLuint tex = 0;

	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(
		tex,
		1,
		format,
		size.x,
		size.y);
	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

static GLenum GetFormat(GLuint tex)
{
	GLenum type;
	glGetTextureLevelParameteriv(tex, 0, GL_TEXTURE_RED_TYPE, reinterpret_cast<GLint*>(&type));

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
	return formats[idx];
}

static const ImVec4 Black(0.0f, 0.0f, 0.0f, 1.0f);

RenderWindow::RenderWindow() :
    Window("Render Window", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize),
    texSize(512,512), editsize(),
    scale(8), wireframe(false), depthtest(true),
    doClearDepth(true), doClearColor(),
	clearDepth(1.0f),
    clearColor(),
    mousePressed(0.0f), mousePos(256,256),
    shownTexture(0)
{
	this->editsize[0] = this->texSize.x;
	this->editsize[1] = this->texSize.y;
	for(int i = 0; i < 4; i++)
	{
		this->tex[i] = 0;
		this->doClearColor[i] = true;
		this->clearColor[i] = Black;
	}

	glCreateRenderbuffers(1, &this->depthbuf);

	glNamedRenderbufferStorage(
		this->depthbuf,
		GL_DEPTH24_STENCIL8,
		this->texSize.x,
		this->texSize.y);

	glCreateFramebuffers(1, &this->fb);

	this->Regen(0, GL_RGBA8);
	this->Regen(1, GL_RGBA8);
	this->Regen(2, GL_RGBA8);
	this->Regen(3, GL_RGBA8);

	glNamedFramebufferRenderbuffer(
		this->fb,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		this->depthbuf);
	auto status = glCheckNamedFramebufferStatus(this->fb, GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
		throw "foo";

	this->geom = this->AddSink<CgDataType::Geometry>("Geometry");
	this->shader = this->AddSink<CgDataType::Shader>("Shader");
    this->transform = this->AddSink<CgDataType::UniformMat4>("Transform");

	this->AddSource(new Source(CgDataType::Texture2D, "Image 0", &this->tex[0]));
	this->AddSource(new Source(CgDataType::Texture2D, "Image 1", &this->tex[1]));
	this->AddSource(new Source(CgDataType::Texture2D, "Image 2", &this->tex[2]));
	this->AddSource(new Source(CgDataType::Texture2D, "Image 3", &this->tex[3]));

	this->AddSource(new Source(CgDataType::UniformVec2, "Image Size", &this->texSize.x));
	this->AddSource(new Source(CgDataType::UniformFloat, "Mouse Pressed", &this->mousePressed));
	this->AddSource(new Source(CgDataType::UniformVec2, "Mouse Pos", &this->mousePos.x));
	this->AddSource(new Source(CgDataType::UniformVec2, "Mouse Pos (Normalized)", &this->mousePosNormalized.x));

    this->drawcalls = this->AddSink<CgDataType::RenderPass>("Drawcalls", Sink::UnlimitedConnections);
}

RenderWindow::~RenderWindow()
{
	glDeleteFramebuffers(1, &this->fb);
	for(int i = 0; i < 4; i++)
		if(this->tex[0] != 0) glDeleteTextures(1, &this->tex[0]);
	glDeleteRenderbuffers(1, &this->depthbuf);
}



void RenderWindow::Render(RenderPass const & pass)
{
    if(pass.shader == nullptr || pass.geometry == nullptr)
        return;

    pass.shader->Use();

    pass.shader->SetTransform(pass.transform);

    glBindVertexArray(pass.geometry->VertexArray);
    glDrawArrays(GL_TRIANGLES, 0, pass.geometry->VertexCount);
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

	glClearDepth(this->clearDepth);
	if(this->doClearDepth)
		glClear(GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < 4; i++)
	{
		if(this->doClearColor[i] == false)
			continue;
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
		glClearColor(
			this->clearColor[i].x,
			this->clearColor[i].y,
			this->clearColor[i].z,
			this->clearColor[i].w);
		glClear(GL_COLOR_BUFFER_BIT);
	}

    GLenum DrawBuffers[4] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
    };
    glDrawBuffers(4, DrawBuffers); // "1" is the size of DrawBuffers

    if(this->wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // TODO: Implement weighted rendering!
    for(int i = 0; i < this->drawcalls->GetSourceCount(); i++)
    {
        RenderPass const & pass = this->drawcalls->GetObject<CgDataType::RenderPass>(i);
        this->Render(pass);
    }

    if(this->shader->HasSource() && this->geom->HasSource())
    {
        RenderPass pass;
        pass.geometry = &this->geom->GetObject<CgDataType::Geometry>();
        pass.shader = &this->shader->GetObject<CgDataType::Shader>();
        pass.transform = this->transform->GetObject<CgDataType::UniformMat4>();
        this->Render(pass);
	}

    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void RenderWindow::SizeConstraint(ImGuiSizeConstraintCallbackData * data)
{
	RenderWindow * window = static_cast<RenderWindow*>(data->UserData);

	float aspect = float(window->texSize.x) / float(window->texSize.y);

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

			ImGui::Separator();

			int size;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);

			ImGui::DragInt2("Size",
				this->editsize,
				1.0f,
				1,
				size);

			if(ImGui::Button("Resize"))
			{
				this->Resize(this->editsize[0], this->editsize[1]);
			}

			ImGui::Separator();

			for(int i = 0; i < 4; i++)
			{
				char buffer[64];
				snprintf(buffer, 64, "Target %d", i);

				ImGui::PushID(&this->tex[i]);
				if(ImGui::BeginMenu(buffer))
				{
					this->ClearEdit(i);
					this->FmtEdit(i);
					this->rtSettings[i].Show();
					ImGui::EndMenu();
				}
				ImGui::PopID();
			}

			ImGui::Separator();

			if(ImGui::BeginMenu("Render Options"))
			{
				ImGui::Checkbox("Wireframe", &this->wireframe);
				ImGui::Checkbox("Depth Test", &this->depthtest);

				ImGui::Separator();

				ImGui::Checkbox("Clear Depth", &this->doClearDepth);

				ImGui::SliderFloat("Depth", &this->clearDepth, 0.0f, 1.0f);

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Display"))
		{
			bool temps[4];
			for(int i = 0; i < 4; i++)
			{
				char buffer[64];
				snprintf(buffer, 64, "Target %d", i);

				temps[i] = (this->shownTexture == i);

				if(ImGui::MenuItem(buffer, nullptr, &temps[i]))
					this->shownTexture = i;
			}

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Export to PGM"))
		{
			for(int i = 0; i < 4; i++)
			{
				char buffer[64];
				snprintf(buffer, 64, "Target %d", i);
				if(ImGui::MenuItem(buffer))
					this->Export(this->tex[i]);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	int w = this->texSize.x;
	int h = this->texSize.y;

	float size = (1 << this->scale);

	auto pos = ImGui::GetCursorPos();
	auto vpsize = ImVec2(size, (size / w) * h);

	ImGui::ImageButton(
		ImTextureID(uintptr_t(this->tex[this->shownTexture])),
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

void RenderWindow::ClearEdit(int idx)
{
	ImGui::Checkbox("Clear", &this->doClearColor[idx]);
	ImGui::ColorEdit4("Color", &this->clearColor[idx].x,
		ImGuiColorEditFlags_AlphaPreviewHalf
	  | ImGuiColorEditFlags_AlphaBar
	  | ImGuiColorEditFlags_Float
	  | ImGuiColorEditFlags_HDR
	  );
}

nlohmann::json RenderWindow::Serialize() const
{
	using namespace nlohmann;

	json array;
	for(int i = 0; i < 4; i++)
	{
		array[i] = {
			{ "format", GetFormat(this->tex[i]) },
		    { "settings", this->rtSettings[i].Serialize() },
		    { "color", { this->clearColor[i].x, this->clearColor[i].y, this->clearColor[i].z, this->clearColor[i].w } },
		    { "clear", this->doClearColor[i] },
		};
	}

	return {
		{ "scale", this->scale },
		{ "wireframe", this->wireframe },
		{ "depthtest", this->depthtest },
		{ "size", { this->texSize.x, this->texSize.y } },
		{ "tex", array }
	};
}

void RenderWindow::Deserialize(const nlohmann::json &value)
{
	this->scale = value["scale"];
	this->wireframe = value.value("wireframe", false);
	this->depthtest = value.value("depthtest", true);

	this->texSize.x = value["size"][0];
	this->texSize.y = value["size"][1];
	this->Resize(this->texSize.x, this->texSize.y);

	auto loadtex = [this](int idx, nlohmann::json const & val)
	{
		try {
			this->rtSettings[idx].Deserialize(val["settings"]);
		} catch(nlohmann::detail::exception const & ex) { }
		this->Regen(idx, val.value("format", GL_RGBA8));

		this->clearColor[idx].x = val["color"][0];
		this->clearColor[idx].y = val["color"][1];
		this->clearColor[idx].z = val["color"][2];
		this->clearColor[idx].w = val["color"][3];

		this->doClearColor[idx] = val["clear"];
	};

	for(int i = 0; i < 4; i++)
		loadtex(0, value["tex"][i]);
}



void RenderWindow::Resize(int w, int h)
{
	this->texSize = ImVec2(w, h);

	glNamedRenderbufferStorage(
		this->depthbuf,
		GL_DEPTH24_STENCIL8,
		this->texSize.x,
		this->texSize.y);

	for(int i = 0; i < 4; i++)
		this->Regen(i, GetFormat(this->tex[i]));
}

void RenderWindow::Export(GLuint tex)
{
	auto path = FileIO::SaveDialog("ppm");
	if(path.empty())
		return;

	int w = this->texSize.x;
	int h = this->texSize.y;

	std::vector<char> bits(w * h * 3);
	glGetTextureImage(
		tex,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		bits.size(),
		bits.data());

	std::ofstream stream(path);
	stream << "P6 " << w << " " << h << " 255" << "\n";
	stream.write(bits.data(), bits.size());
	stream.flush();
}

void RenderWindow::FmtEdit(int idx)
{
	GLenum type;
	glGetTextureLevelParameteriv(this->tex[idx], 0, GL_TEXTURE_RED_TYPE, reinterpret_cast<GLint*>(&type));

	int fmt = 0;
	switch(type)
	{
		case GL_UNSIGNED_NORMALIZED: fmt = 0; break;
		case GL_FLOAT:               fmt = 1; break;
		case GL_INT:                 fmt = 2; break;
		case GL_UNSIGNED_INT:        fmt = 3; break;
		default:                     fmt = 0; break;
	}
	static char const * names[] = {
		"default",
	    "float",
	    "int",
	    "uint"
	};
	static GLenum formats[] = {
	    GL_RGBA8, GL_RGBA32F, GL_RGBA8I, GL_RGBA8UI
	};

	if(ImGui::Combo("Format", &fmt, names, 4))
		this->Regen(idx, formats[fmt]);
}

void RenderWindow::Regen(int idx, GLenum format)
{
	if(this->tex[idx])
		glDeleteTextures(1, &this->tex[idx]);

	this->tex[idx] = gentex(this->texSize, format);

	rtSettings[idx].SetTexture(this->tex[idx]);

	glNamedFramebufferTexture(
		this->fb,
		GL_COLOR_ATTACHMENT0 + idx,
	    this->tex[idx],
		0);
}

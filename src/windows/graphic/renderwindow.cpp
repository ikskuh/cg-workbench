#include "renderwindow.hpp"
#include <nfd.h>
#include <fstream>
#include <unistd.h>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(RenderWindow, Menu, "renderer", "Renderer");

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

RenderWindow::RenderWindow() :
    Window("Render Window", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize),
    tex0(0), tex1(0), tex2(0), tex3(0),
    texSize(512,512), editsize({512,512}),
    scale(8), wireframe(false), depthtest(true),
    mousePressed(0.0f), mousePos(256,256),
    shownTexture(0)
{
	glCreateRenderbuffers(1, &this->depthbuf);

	glNamedRenderbufferStorage(
		this->depthbuf,
		GL_DEPTH24_STENCIL8,
		this->texSize.x,
		this->texSize.y);

	glCreateFramebuffers(1, &this->fb);

	this->Regen(this->tex0, GL_RGBA8);
	this->Regen(this->tex1, GL_RGBA8);
	this->Regen(this->tex2, GL_RGBA8);
	this->Regen(this->tex3, GL_RGBA8);

	glNamedFramebufferRenderbuffer(
		this->fb,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		this->depthbuf);
	auto status = glCheckNamedFramebufferStatus(this->fb, GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
		throw "foo";

	rt0Settings.SetTexture(this->tex0);
	rt1Settings.SetTexture(this->tex1);
	rt2Settings.SetTexture(this->tex2);
	rt3Settings.SetTexture(this->tex3);

	this->AddSink(this->geom = new Sink(CgDataType::Geometry, "Geometry"));
	this->AddSink(this->shader = new Sink(CgDataType::Shader, "Shader"));

	this->AddSource(new Source(CgDataType::Texture2D, "Image 0", &this->tex0));
	this->AddSource(new Source(CgDataType::Texture2D, "Image 1", &this->tex1));
	this->AddSource(new Source(CgDataType::Texture2D, "Image 2", &this->tex2));
	this->AddSource(new Source(CgDataType::Texture2D, "Image 3", &this->tex3));

	this->AddSource(new Source(CgDataType::UniformVec2, "Image Size", &this->texSize.x));
	this->AddSource(new Source(CgDataType::UniformFloat, "Mouse Pressed", &this->mousePressed));
	this->AddSource(new Source(CgDataType::UniformVec2, "Mouse Pos", &this->mousePos.x));
	this->AddSource(new Source(CgDataType::UniformVec2, "Mouse Pos (Normalized)", &this->mousePosNormalized.x));
}

RenderWindow::~RenderWindow()
{
	glDeleteFramebuffers(1, &this->fb);
	if(this->tex0 != 0) glDeleteTextures(1, &this->tex0);
	if(this->tex1 != 0) glDeleteTextures(1, &this->tex1);
	if(this->tex2 != 0) glDeleteTextures(1, &this->tex2);
	if(this->tex3 != 0) glDeleteTextures(1, &this->tex3);
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

		this->shader->GetObject<CgDataType::Shader>().Use();

		glBindVertexArray(geom.VertexArray);

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

		glDrawArrays(GL_TRIANGLES, 0, geom.VertexCount);

		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
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

			if(ImGui::BeginMenu("Target 0"))
			{
				this->FmtEdit(this->tex0);
				this->rt0Settings.Show();
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Target 1"))
			{
				this->FmtEdit(this->tex1);
				this->rt1Settings.Show();
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Target 2"))
			{
				this->FmtEdit(this->tex2);
				this->rt2Settings.Show();
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Target 3"))
			{
				this->FmtEdit(this->tex3);
				this->rt3Settings.Show();
				ImGui::EndMenu();
			}

			ImGui::Separator();

			ImGui::MenuItem("Wireframe", nullptr, &this->wireframe);
			ImGui::MenuItem("Depth Test", nullptr, &this->depthtest);

			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Display"))
		{
			bool b0 = (this->shownTexture == 0);
			bool b1 = (this->shownTexture == 1);
			bool b2 = (this->shownTexture == 2);
			bool b3 = (this->shownTexture == 3);

			if(ImGui::MenuItem("Target 0", nullptr, &b0)) this->shownTexture = 0;
			if(ImGui::MenuItem("Target 1", nullptr, &b1)) this->shownTexture = 1;
			if(ImGui::MenuItem("Target 2", nullptr, &b2)) this->shownTexture = 2;
			if(ImGui::MenuItem("Target 3", nullptr, &b3)) this->shownTexture = 3;

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Export to PGM"))
		{
			if(ImGui::MenuItem("Target 0")) this->Export(this->tex0);
			if(ImGui::MenuItem("Target 1")) this->Export(this->tex1);
			if(ImGui::MenuItem("Target 2")) this->Export(this->tex2);
			if(ImGui::MenuItem("Target 3")) this->Export(this->tex3);
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	int w = this->texSize.x;
	int h = this->texSize.y;

	float size = (1 << this->scale);

	auto pos = ImGui::GetCursorPos();
	auto vpsize = ImVec2(size, (size / w) * h);
	GLuint tex = this->tex0;
	switch(this->shownTexture)
	{
		case 0: tex = this->tex0; break;
		case 1: tex = this->tex1; break;
		case 2: tex = this->tex2; break;
		case 3: tex = this->tex3; break;
	}

	ImGui::ImageButton(
		ImTextureID(uintptr_t(tex)),
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
		{ "size", { this->texSize.x, this->texSize.y } },
		{ "tex-0", GetFormat(this->tex0) },
		{ "tex-1", GetFormat(this->tex1) },
		{ "tex-2", GetFormat(this->tex2) },
		{ "tex-3", GetFormat(this->tex3) },
		{ "rt0-settings", this->rt0Settings.Serialize() },
		{ "rt1-settings", this->rt1Settings.Serialize() },
		{ "rt2-settings", this->rt2Settings.Serialize() },
		{ "rt3-settings", this->rt3Settings.Serialize() },
	};
}

void RenderWindow::Deserialize(const nlohmann::json &value)
{
	this->scale = value["scale"];
	this->wireframe = value.value("wireframe", false);
	this->depthtest = value.value("depthtest", true);

	try
	{
		if(value.find("size") != value.end())
		{
			this->texSize.x = value["size"][0];
			this->texSize.y = value["size"][1];

			this->Resize(this->texSize.x, this->texSize.y);
		}
	} catch(nlohmann::detail::exception const & ex) { }

	try {
		this->rt0Settings.Deserialize(value.at("rt0-settings"));
	} catch(nlohmann::detail::exception const & ex) { }

	try {
		this->rt1Settings.Deserialize(value.at("rt1-settings"));
	} catch(nlohmann::detail::exception const & ex) { }

	try {
		this->rt2Settings.Deserialize(value.at("rt2-settings"));
	} catch(nlohmann::detail::exception const & ex) { }

	try {
		this->rt3Settings.Deserialize(value.at("rt3-settings"));
	} catch(nlohmann::detail::exception const & ex) { }

	this->Regen(this->tex0, value.value("tex-0", GL_RGBA8));
	this->Regen(this->tex1, value.value("tex-1", GL_RGBA8));
	this->Regen(this->tex2, value.value("tex-2", GL_RGBA8));
	this->Regen(this->tex3, value.value("tex-3", GL_RGBA8));
}



void RenderWindow::Resize(int w, int h)
{
	this->texSize = ImVec2(w, h);

	glNamedRenderbufferStorage(
		this->depthbuf,
		GL_DEPTH24_STENCIL8,
		this->texSize.x,
		this->texSize.y);

	this->Regen(this->tex0, GetFormat(this->tex0));
	this->Regen(this->tex1, GetFormat(this->tex1));
	this->Regen(this->tex2, GetFormat(this->tex2));
	this->Regen(this->tex3, GetFormat(this->tex3));
}

void RenderWindow::Export(GLuint tex)
{
	char cwd[256];
	nfdchar_t *outPath = NULL;
	nfdresult_t result = NFD_SaveDialog("ppm", getcwd(cwd, sizeof(cwd)), &outPath );
	if ( result == NFD_OKAY )
	{
		std::string fileName(outPath);
		free(outPath);

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

void RenderWindow::FmtEdit(GLuint & tex)
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
	static char const * names[] = {
		"default",
	    "float",
	    "int",
	    "uint"
	};
	static GLenum formats[] = {
	    GL_RGBA8, GL_RGBA32F, GL_RGBA8I, GL_RGBA8UI
	};

	if(ImGui::Combo("Format", &idx, names, 4))
		this->Regen(tex, formats[idx]);
}

void RenderWindow::Regen(GLuint & tex, GLenum format)
{
	if(tex)
		glDeleteTextures(1, &tex);

	tex = gentex(this->texSize, format);

	GLenum target;
	if(&tex == &this->tex0) {
		target = GL_COLOR_ATTACHMENT0;
		rt0Settings.SetTexture(tex);
	}
	else if(&tex == &this->tex1) {
		target = GL_COLOR_ATTACHMENT1;
		rt1Settings.SetTexture(tex);
	}
	else if(&tex == &this->tex2) {
		target = GL_COLOR_ATTACHMENT2;
		rt2Settings.SetTexture(tex);
	}
	else if(&tex == &this->tex3) {
		target = GL_COLOR_ATTACHMENT3;
		rt3Settings.SetTexture(tex);
	}

	glNamedFramebufferTexture(
		this->fb,
		target,
	    tex,
		0);
}

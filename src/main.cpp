// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include <stdio.h>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <SDL.h>
#include "imgui_impl.h"
#include <nfd.h>
#include <unistd.h>
#include <json.hpp>
#include <fstream>

#include "window.hpp"

#include "windows/shadereditor.hpp"
#include "windows/renderwindow.hpp"
#include "windows/luaconsole.hpp"
#include "windows/geometrywindow.hpp"
#include "windows/gpuerrorlog.hpp"
#include "windows/uniformwindow.hpp"
#include "windows/colorwindow.hpp"
#include "windows/timerwindow.hpp"
#include "windows/arithmeticwindow.hpp"
#include "windows/notewindow.hpp"
#include "windows/bufferwindow.hpp"
#include "windows/graphwindow.hpp"
#include "windows/vectoradapter.hpp"
#include "windows/imagesource.hpp"
#include "windows/trigger.hpp"
#include "windows/imagebuffer.hpp"
#include "resources.hpp"

std::string currentFileName;

void save(std::string const & fileName);

void load(std::string const & fileName);

extern int currentWindowID;

static Window * createMenu()
{
	Window * result = nullptr;
	if(ImGui::MenuItem("Shader"))   result = new ShaderEditor();
	if(ImGui::MenuItem("Geometry")) result = new GeometryWindow();
	if(ImGui::MenuItem("Renderer")) result = new RenderWindow();
	if(ImGui::MenuItem("Graph")) result = new GraphWindow();
	if(ImGui::MenuItem("Image")) result = new ImageSource();
	if(ImGui::MenuItem("Trigger")) result = new Trigger();
	if(ImGui::BeginMenu("Values"))
	{
		if(ImGui::MenuItem("Float")) result = new UniformWindow<CgDataType::UniformFloat>();
		ImGui::Separator();
		if(ImGui::MenuItem("Vec2")) result = new UniformWindow<CgDataType::UniformVec2>();
		if(ImGui::MenuItem("Vec3")) result = new UniformWindow<CgDataType::UniformVec3>();
		if(ImGui::MenuItem("Vec4")) result = new UniformWindow<CgDataType::UniformVec4>();
		ImGui::Separator();
		if(ImGui::MenuItem("Mat3")) result = new UniformWindow<CgDataType::UniformMat3>();
		if(ImGui::MenuItem("Mat4")) result = new UniformWindow<CgDataType::UniformMat4>();
		ImGui::Separator();
		if(ImGui::MenuItem("Color")) result = new ColorWindow();
		if(ImGui::MenuItem("Timer")) result = new TimerWindow();

		ImGui::EndMenu();
	}
	if(ImGui::BeginMenu("Arithmetic"))
	{
		if(ImGui::MenuItem("float")) result = new ArithmeticWindow<CgDataType::UniformFloat>();
		if(ImGui::MenuItem("vec2")) result = new ArithmeticWindow<CgDataType::UniformVec2>();
		if(ImGui::MenuItem("vec3")) result = new ArithmeticWindow<CgDataType::UniformVec3>();
		if(ImGui::MenuItem("vec4")) result = new ArithmeticWindow<CgDataType::UniformVec4>();
		ImGui::Separator();
		if(ImGui::MenuItem("vec to float")) result = new VectorSplitter();
		if(ImGui::MenuItem("float to vec")) result = new VectorMerger();
		ImGui::EndMenu();
	}
	if(ImGui::BeginMenu("Buffer"))
	{
		if(ImGui::MenuItem("float")) result = new BufferWindow<CgDataType::UniformFloat>();
		if(ImGui::MenuItem("vec2")) result = new BufferWindow<CgDataType::UniformVec2>();
		if(ImGui::MenuItem("vec3")) result = new BufferWindow<CgDataType::UniformVec3>();
		if(ImGui::MenuItem("vec4")) result = new BufferWindow<CgDataType::UniformVec4>();
		ImGui::Separator();
		if(ImGui::MenuItem("Image")) result = new ImageBuffer();
		ImGui::EndMenu();
	}

	ImGui::Separator();
	if(ImGui::MenuItem("Note")) result = new NoteWindow();

	if(result)
		Window::Register(result);
	return result;
}

extern ImVec2 screen_pan;

int main(int argc, char ** argv)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

	SDL_Window *window = SDL_CreateWindow(
		"CG Workbench *FLOAT*",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 720,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplSdlGL3_Init(window);

    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    bool show_test_window = false;
	bool show_style_editor = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(&GpuErrorLog::LogMessage, nullptr);

	resources::load("/home/felix/projects/cg-workbench/");

	if(argc == 2)
	{
		load(std::string(argv[1]));
	}

    // Main loop
    bool done = false;
	static char cwd[256];
	ImVec2 spawn;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
			if(event.type == SDL_MOUSEMOTION && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_MIDDLE)))
			{
				screen_pan.x -= event.motion.xrel;
				screen_pan.y -= event.motion.yrel;

				if(screen_pan.x < 0) screen_pan.x = 0;
				if(screen_pan.y < 0) screen_pan.y = 0;

				if(screen_pan.x > 10000) screen_pan.x = 10000;
				if(screen_pan.y > 10000) screen_pan.y = 10000;
			}

            ImGui_ImplSdlGL3_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }

		Window::RenderAll();

        ImGui_ImplSdlGL3_NewFrame(window);

		Window::UpdateAll();

		{
			int w,h;
			SDL_GetWindowSize(window, &w, &h);

			ImGui::SetNextWindowPos(screen_pan, ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

			auto flags = ImGuiWindowFlags_NoTitleBar
			        | ImGuiWindowFlags_MenuBar
					| ImGuiWindowFlags_NoBringToFrontOnFocus
					| ImGuiWindowFlags_NoCollapse
					| ImGuiWindowFlags_NoFocusOnAppearing
					| ImGuiWindowFlags_NoMove
					| ImGuiWindowFlags_NoResize
					| ImGuiWindowFlags_NoSavedSettings
					| ImGuiWindowFlags_NoScrollbar;

			if(ImGui::Begin("Desktop", nullptr, flags))
			{
				if (ImGui::BeginMenuBar())
			    {
			        if (ImGui::BeginMenu("File"))
			        {
						if(ImGui::MenuItem("New", "CTRL+N"))
						{
							Window::DestroyAll();
							currentWindowID = 0; // reset window id counter
						}

						if(ImGui::MenuItem("Open...", "CTRL+O"))
						{
							nfdchar_t *outPath = NULL;
							nfdresult_t result = NFD_OpenDialog("json", getcwd(cwd, sizeof(cwd)), &outPath );
							if ( result == NFD_OKAY )
							{
								currentFileName = std::string(outPath);
								free(outPath);
								load(currentFileName);
							}
							else if ( result == NFD_CANCEL )
								; // Silently ignore cancel
							else
							{
								printf("Error: %s\n", NFD_GetError() );
							}
						}
						ImGui::Separator();

						bool requiresSaveAs = false;
						if(ImGui::MenuItem("Save", "CTRL+S"))
						{
							if(currentFileName.length() == 0)
								requiresSaveAs = true;
							else
								save(currentFileName);
						}
						if(ImGui::MenuItem("Save As...", "CTRL+SHIFT+S") || requiresSaveAs)
						{
							nfdchar_t *outPath = NULL;
							nfdresult_t result = NFD_SaveDialog("json", getcwd(cwd, sizeof(cwd)), &outPath );
							if ( result == NFD_OKAY )
							{
								currentFileName = std::string(outPath);
								free(outPath);
								save(currentFileName);
							}
							else if ( result == NFD_CANCEL )
								; // Silently ignore cancel
							else
							{
								printf("Error: %s\n", NFD_GetError() );
							}
						}
						ImGui::Separator();
						if(ImGui::MenuItem("Exit", "ALT+F4"))
						{
							done = true;
						}
			            ImGui::EndMenu();
			        }
			        if (ImGui::BeginMenu("Create"))
			        {
						createMenu();
			            ImGui::EndMenu();
			        }
					if(ImGui::BeginMenu("Extras"))
					{
						if(ImGui::MenuItem("Lua Console")) Window::Register(new LuaConsole());
						if(ImGui::MenuItem("OpenGL Log")) Window::Register(new GpuErrorLog());
						ImGui::Separator();
						if(ImGui::MenuItem("Show Test Window", nullptr, &show_test_window)) { }
						if(ImGui::MenuItem("Show Style Editor", nullptr, &show_style_editor)) { }

						ImGui::EndMenu();
					}
			        ImGui::EndMenuBar();
			    }
				if(ImGui::BeginPopupContextWindow(nullptr, 1))
				{
					Window * win = createMenu();
					if(win != nullptr)
					{
						win->wantsResize = true;
						win->pos = spawn;
					}
					ImGui::EndPopup();
				}
				else
				{
					spawn = ImGui::GetIO().MousePos;
				}
				Window::UpdateNodes();
			}
			ImGui::End();

			ImGui::PopStyleVar();
		}


        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
			ImGui::ShowTestWindow(&show_test_window);
        }

		if(show_style_editor)
		{
			ImGui::Begin("Style Editor", &show_style_editor);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}

        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window);
    }

	Window::DestroyAll();

    // Cleanup
    ImGui_ImplSdlGL3_Shutdown();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void save(std::string const & fileName)
{
	using namespace nlohmann;
	json file;

	file["type"] = "cg-workbench-file";
	file["version"] = "1.0";

	json windows = { };
	json signals = { };

	for(auto it = Window::Begin(); it != Window::End(); it++)
	{
		Window * win = (*it).get();

		json window = win->Serialize();
		if(window.is_null())
			continue;

		window["window-id"] = win->GetID();
		window["window-pos"] = {
			win->GetPosition().x,
			win->GetPosition().y
		};
		window["window-size"] = {
			win->GetSize().x,
			win->GetSize().y
		};

		windows += window;

		for(int i = 0; i < win->GetSinkCount(); i++)
		{
			Sink * sink = win->GetSink(i);
			if(sink->HasSourceConnected())
			{
				signals +=
				{
					{
						"from",
						{
							sink->GetSource(false)->GetWindow()->GetID(),
							sink->GetSource(false)->GetWindowIndex()
						}
					},
					{
						"to",
						{
							sink->GetWindow()->GetID(),
							sink->GetWindowIndex(),
						}
					}
				};
			}
		}
	}

	file["windows"] = windows;
	file["signals"] = signals;

	std::ofstream stream(fileName);
	stream << std::setw(4) << file << std::endl;
	stream.flush();
}

void load(std::string const & fileName)
{
	using namespace nlohmann;

	json file;
	try
	{
		std::ifstream stream(fileName);
		if(stream.is_open() == false)
			return;
		stream >> file;
	}
	catch(nlohmann::detail::parse_error const & pe)
	{
		printf("Parse Error: %s\n", pe.what());
		return;
	}

	if(file["type"] != "cg-workbench-file")
		return;
	if(file["version"] != "1.0")
		return;
	json windows = file["windows"];
	json signals = file["signals"];

	std::map<int, Window*> windowById;

	Window::DestroyAll();
	currentWindowID = 0;

	for(json window : windows)
	{
		Window * win = nullptr;

		// renderer -> RenderWindow
		// geometry -> GeometryWindow
		// shader   -> ShaderEditor
		json type = window["type"];
		if(type == "renderer")
			win = new RenderWindow();
		else if(type == "geometry")
			win = new GeometryWindow();
		else if(type == "shader")
			win = new ShaderEditor();
		else if(type == "uniform-float")
			win = new UniformWindow<CgDataType::UniformFloat>();
		else if(type == "uniform-vec2")
			win = new UniformWindow<CgDataType::UniformVec2>();
		else if(type == "uniform-vec3")
			win = new UniformWindow<CgDataType::UniformVec3>();
		else if(type == "uniform-vec4")
			win = new UniformWindow<CgDataType::UniformVec4>();
		else if(type == "uniform-mat3")
			win = new UniformWindow<CgDataType::UniformMat3>();
		else if(type == "uniform-mat4")
			win = new UniformWindow<CgDataType::UniformMat4>();
		else if(type == "uniform-color")
			win = new ColorWindow();
		else if(type == "timer")
			win = new TimerWindow();
		else if(type == "arithmetic-float")
			win = new ArithmeticWindow<CgDataType::UniformFloat>();
		else if(type == "arithmetic-vec2")
			win = new ArithmeticWindow<CgDataType::UniformVec2>();
		else if(type == "arithmetic-vec3")
			win = new ArithmeticWindow<CgDataType::UniformVec3>();
		else if(type == "arithmetic-vec4")
			win = new ArithmeticWindow<CgDataType::UniformVec4>();
		else if(type == "buffer-float")
			win = new BufferWindow<CgDataType::UniformFloat>();
		else if(type == "buffer-vec2")
			win = new BufferWindow<CgDataType::UniformVec2>();
		else if(type == "buffer-vec3")
			win = new BufferWindow<CgDataType::UniformVec3>();
		else if(type == "buffer-vec4")
			win = new BufferWindow<CgDataType::UniformVec4>();
		else if(type == "note")
			win = new NoteWindow();
		else if(type == "graph")
			win = new GraphWindow();
		else if(type == "vector-splitter")
			win = new VectorSplitter();
		else if(type == "vector-merger")
			win = new VectorMerger();
		else if(type == "image")
			win = new ImageSource();
		else if(type == "trigger")
			win = new Trigger();
		else if(type == "image-buffer")
			win = new ImageBuffer();
		else
			abort();

		json pos  = window["window-pos"];
		json size = window["window-size"];
		win->wantsResize = true;
		win->pos  = ImVec2(pos[0], pos[1]);
		win->size = ImVec2(size[0], size[1]);
		win->id =   window["window-id"];

		win->Deserialize(window);

		Window::Register(win);
		windowById[win->id] = win;

		currentWindowID = std::max(currentWindowID, win->GetID());
	}

	for(json con : signals)
	{
		json from = con["from"];
		json to = con["to"];

		Window * src = windowById.at(from[0]);
		Window * dst = windowById.at(to[0]);

		dst->GetSink(to[1])->SetSource(src->GetSource(from[1]));
	}
}
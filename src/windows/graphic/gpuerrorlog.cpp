#include "gpuerrorlog.hpp"
#include <mutex>
#include <signal.h>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(GpuErrorLog, Menu::Tools, "gpu-error-log", "OpenGL Log");

struct logentry
{
	GLenum source;
	GLenum type;
	GLuint id;
	GLenum severity;
	std::string message;
};

static std::mutex mutex;
static std::vector<logentry> entries;

static bool breakOnNextError = true;

void APIENTRY GpuErrorLog::LogMessage(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam)
{
	(void)userParam;
	std::lock_guard<std::mutex> lock(mutex);

	logentry log;
	log.source = source;
	log.type = type;
	log.id = id;
	log.severity = severity;
	log.message = std::string(message, length);
	entries.insert(entries.begin(), log);

	if(entries.size() > 1000)
		entries.pop_back();

	fprintf(stderr, "[GL] %s\n", log.message.c_str());

	if(breakOnNextError && severity == GL_DEBUG_SEVERITY_HIGH)
	{
#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak()
#else
		raise(SIGTRAP);
#endif
	}
	breakOnNextError = false;
}

GpuErrorLog::GpuErrorLog() :
    Window("OpenGL Error Log")
{

}

GpuErrorLog::~GpuErrorLog()
{

}

void GpuErrorLog::OnUpdate()
{
	bool copyToClipboard = false;
	bool scrollToEnd = false;

	bool logEnabled = glIsEnabled(GL_DEBUG_OUTPUT);

	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Log"))
		{
			// if(ImGui::MenuItem("Copy to clipboard")) copyToClipboard = true;
			// if(ImGui::MenuItem("Scroll to end")) scrollToEnd = true;
			ImGui::MenuItem("Enabled", nullptr, &logEnabled);
			ImGui::Separator();
			if(ImGui::MenuItem("Close")) this->Close();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if(logEnabled != glIsEnabled(GL_DEBUG_OUTPUT))
	{
		if(logEnabled)
			glEnable(GL_DEBUG_OUTPUT);
		else
			glDisable(GL_DEBUG_OUTPUT);
	}

    ImGui::BeginChild("ScrollingRegion", ImVec2(0,-ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);

    // Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
    // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
    // You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
    // To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
    //     ImGuiListClipper clipper(Items.Size);
    //     while (clipper.Step())
    //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
    // However take note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
    // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
    // and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
    // If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
    if (copyToClipboard)
		ImGui::LogToClipboard();

	std::lock_guard<std::mutex> lock(mutex);
    for(auto const & log : entries)
    {
        ImVec4 col = ImVec4(1.0f,1.0f,1.0f,1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
		switch(log.severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				col = ImColor(1.0f,0.4f,0.4f,1.0f);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				col = ImColor(0.8f,1.0f,0.0f,1.0f);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				col = ImColor(0.4f,1.0f,0.0f,1.0f);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				col = ImColor(0.8f,0.8f,0.8f,1.0f);
				break;
		}
        // else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f,0.78f,0.58f,1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::TextWrapped("%s", log.message.c_str());
        ImGui::PopStyleColor();
    }
    if (copyToClipboard)
        ImGui::LogFinish();

    if (scrollToEnd)
		ImGui::SetScrollHere();

    ImGui::PopStyleVar();
    ImGui::EndChild();
}

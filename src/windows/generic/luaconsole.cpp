#include "luaconsole.hpp"
extern "C"
{
#include <lauxlib.h>
#include <lualib.h>
}

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(LuaConsole, Menu::Tools, "lua-console", "Lua Console")


LuaConsole::LuaConsole() :
    Window("Lua Console"),
    log(),
    scrollToEnd(true),
    lua(luaL_newstate()),
	inputBuffer()
{
	assert(this->lua);
	luaL_openlibs(this->lua);

	lua_pushlightuserdata(this->lua, this);
	lua_pushcclosure(this->lua, &LuaConsole::LuaPrint, 1);
	lua_setglobal(this->lua, "print");

	strcpy(this->inputBuffer, "");
}

LuaConsole::~LuaConsole()
{
	lua_close(this->lua);
}

void LuaConsole::OnUpdate()
{
	bool copyToClipboard = false;

	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Console"))
		{
			if(ImGui::MenuItem("Copy to clipboard")) copyToClipboard = true;
			if(ImGui::MenuItem("Scroll to end")) this->scrollToEnd = true;
			ImGui::Separator();
			if(ImGui::MenuItem("Close")) this->Close();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
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
    for (size_t i = 0; i < this->log.size(); i++)
    {
        ImVec4 col = ImVec4(1.0f,1.0f,1.0f,1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
        if (this->log[i].isError)
			col = ImColor(1.0f,0.4f,0.4f,1.0f);
        // else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f,0.78f,0.58f,1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::TextUnformatted(this->log[i].message.c_str());
        ImGui::PopStyleColor();
    }
    if (copyToClipboard)
        ImGui::LogFinish();

    if (this->scrollToEnd)
		ImGui::SetScrollHere();
	this->scrollToEnd = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    if (ImGui::InputText("Input", this->inputBuffer, LuaConsole::MaxInputLength, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        char* input_end = this->inputBuffer + strlen(this->inputBuffer);
        while (input_end > this->inputBuffer && input_end[-1] == ' ') { input_end--; } *input_end = 0;
        if (this->inputBuffer[0])
            this->Execute(this->inputBuffer);
        strcpy(this->inputBuffer, "");
    }

    // Demonstrate keeping auto focus on the input box
    if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
}

void LuaConsole::Execute(const char *str)
{
	int start = lua_gettop(this->lua) + 1;
	if(!luaL_dostring(this->lua, str))
	{
		// good
		int end = lua_gettop(this->lua);
		std::string result = "";
		for (int i = start; i <= end; i++)
		{
			if(i > start)
				result += "\t";
			result += lua_tostring(lua, i);
		}
		if(result.length() > 0)
			this->Print(result.c_str());
	}
	else
	{
		// bad
		char const * message = lua_tostring(this->lua, -1);

		this->Print(message, true);

		lua_pop(this->lua, 1);  /* pop error message from the stack */
	}
}

void LuaConsole::Print(char const * str, bool err)
{
    logentry log;
    log.isError = err;
    log.message = str;
    this->log.emplace_back(log);
}

int LuaConsole::LuaPrint(lua_State * lua)
{
	LuaConsole * console = reinterpret_cast<LuaConsole*>(lua_touserdata(lua, lua_upvalueindex(1)));

	std::string result = "";

	int n = lua_gettop(lua);    /* number of arguments */
	for (int i = 1; i <= n; i++)
	{
		if(i > 1)
			result += "\t";
		result += lua_tostring(lua, i);
	}

	console->Print(result.c_str());
	return 0;
}

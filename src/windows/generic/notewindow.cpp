#include "notewindow.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(NoteWindow, Menu::Tools, "note", "Note");

NoteWindow::NoteWindow() :
    Window("Notes", 0)
{

}

void NoteWindow::OnUpdate()
{
	ImGui::InputTextMultiline(
		"##text",
		this->buffer, NoteWindow::MaxLength,
		ImVec2(-1.0f, ImGui::GetTextLineHeight() * 14),
		ImGuiInputTextFlags_AllowTabInput);
}

nlohmann::json NoteWindow::Serialize() const
{
	return {
		{ "text", std::string(this->buffer) }
	};
}

void NoteWindow::Deserialize(const nlohmann::json &value)
{
	strcpy(this->buffer, value.value("text", "").c_str());
}

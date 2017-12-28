#include "notewindow.hpp"

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
		{ "type", "note" },
		{ "text", std::string(this->buffer) }
	};
}

void NoteWindow::Deserialize(const nlohmann::json &value)
{
	strcpy(this->buffer, value["text"].get<std::string>().c_str());
}

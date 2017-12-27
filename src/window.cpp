#include "window.hpp"
#include <vector>
#include <algorithm>
#include <memory>
#include <cstdio>

static std::vector<std::unique_ptr<Window>> windows;

int currentWindowID = 0;

std::vector<std::unique_ptr<Window>>::iterator Window::Begin() { return windows.begin(); }
std::vector<std::unique_ptr<Window>>::iterator Window::End()   { return windows.end(); }

void Window::Register(Window * window)
{
	auto pos = std::find_if(windows.begin(), windows.end(), [window](std::unique_ptr<Window> const & p) {
		return (p.get() == window);
	});
	if(pos == windows.end())
		windows.emplace_back(window);
}

void Window::Unregister(Window * window)
{
	auto pos = std::remove_if(windows.begin(), windows.end(), [window](std::unique_ptr<Window> const & p) {
		return p.get() == window;
	});
	for(auto i = pos; i != windows.end(); i++)
		i->release();
	windows.erase(pos, windows.end());
}

void Window::UpdateAll()
{
	for(auto const & win : windows)
		win->Update();

	auto pos = std::remove_if(windows.begin(), windows.end(), [](std::unique_ptr<Window> const & win) {
		return !win->isOpen;
	});
	windows.erase(pos, windows.end());
}

void Window::UpdateNodes()
{
	static Source * currentSource = nullptr;
	static Sink * currentSink = nullptr;

	int offset;
	int margin = 4;
	int size = 24;

	auto * draw = ImGui::GetWindowDrawList();
	auto & io = ImGui::GetIO();

	currentSink = nullptr;
	for(auto const & win : windows)
	{
		for(auto const & sink : win->sinks)
		{
			ImVec2 pos(
				win->pos.x - size - margin,
				win->pos.y + margin + sink->GetWindowIndex() * (margin + size));

			ImVec2 m = io.MousePos;
			bool hovered = (m.x >= pos.x)
			        && (m.x < (pos.x + size))
			        && (m.y >= pos.y)
			        && (m.y < (pos.y + size));

			ImGui::SetCursorScreenPos(pos);
			if(currentSource != nullptr)
			{
				ImVec4 color(1.0f,0.0f,0.0f,1.0f);
				if(sink->GetType() == currentSource->GetType())
				{
					if(hovered)
					{
						currentSink = sink.get();
						color = ImVec4(1.0f,1.0f,0.0f,1.0f);
					}
					else
					{
						color = ImVec4(0.0f,1.0f,0.0f,1.0f);
					}
				}
				ImGui::PushStyleColor(ImGuiCol_Button, color);
			}
			if(ImGui::Button(MakeTitle("",sink.get()), ImVec2(size, size)))
				sink->SetSource(nullptr);
			if(currentSource != nullptr)
			{
				ImGui::PopStyleColor();
			}
			if(ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", sink->GetName().c_str());

			if(sink->GetSource(false) != nullptr)
			{
				auto * src = sink->GetSource();
				auto * srcwin = src->GetWindow();
				auto srcpos = srcwin->GetPosition();
				auto srcsize = srcwin->GetSize();
				int index = src->GetWindowIndex();

				ImVec2 from(srcpos.x + srcsize.x + margin + size, srcpos.y + margin + (size + margin) * index + size / 2);
				ImVec2 to(pos.x, pos.y + size / 2);

				draw->AddBezierCurve(
					from,
				    ImVec2(from.x + 64, from.y),
					ImVec2(to.x - 64, to.y),
					to,
					ImColor(0.7f,0.7f,0.7f,1.0f),
					2.0f);
			}
		}
	}

	for(auto const & win : windows)
	{
		offset = margin;
		for(auto const & source : win->sources)
		{
			ImGui::SetCursorScreenPos(ImVec2(
				win->pos.x + win->size.x + margin,
				win->pos.y + offset));
			ImGui::Button(MakeTitle("",source.get()), ImVec2(size, size));
			if(ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", source->GetName().c_str());
			if(ImGui::IsItemActive())
			{
				currentSource = source.get();
				draw->PushClipRectFullScreen();
				draw->AddLine(
					ImGui::CalcItemRectClosestPoint(io.MousePos, true, -2.0f),
					io.MousePos,
					ImColor(1.0f, 0.0f, 0.0f, 1.0f),
					4.0f);
				draw->PopClipRect();
			}
			else if(currentSource == source.get())
			{
				if(currentSink != nullptr)
				{
					currentSink->SetSource(currentSource);
				}
				currentSource = nullptr;
			}
			offset += size + margin;
		}
	}
}

void Window::RenderAll()
{
	for(auto const & win : windows)
		win->Render();
}

void Window::DestroyAll()
{
	windows.clear();
}

Window::Window(std::string const & title, ImGuiWindowFlags flags) :
    id(++currentWindowID),
	isOpen(true),
    title(title),
    flags(flags),
    wantsResize(false)
{

}

Window::~Window()
{
	for(auto const & win : windows)
	{
		if(win.get() == nullptr)
			continue;
		if(win.get() == this)
			continue;
		for(auto const & source : this->sources)
		{
			for(auto const & sink : win->sinks)
			{
				if(sink->GetSource() == source.get())
					sink->SetSource(nullptr);
			}
		}
	}
}

void Window::Update()
{
	this->OnSetup();

	if(this->wantsResize)
	{
		ImGui::SetNextWindowPos(this->pos, ImGuiCond_Once);
		ImGui::SetNextWindowSize(this->size, ImGuiCond_Once);
		this->wantsResize = false;
	}

	if(ImGui::Begin(MakeTitle(this->title.c_str()), &this->isOpen, this->flags))
	{
		this->OnUpdate();

		this->pos = ImGui::GetWindowPos();
		this->size = ImGui::GetWindowSize();
	}
	ImGui::End();
}

void Window::OnSetup()
{
	ImGui::SetNextWindowSize(ImVec2(256, 256), ImGuiCond_FirstUseEver);
}

void Window::OnRender()
{

}

void Window::Render()
{
	this->OnRender();
}

void Window::Close()
{
	this->isOpen = false;
}

char const * Window::MakeTitle(char const * text)
{
	return MakeTitle(text, this);
}

char const * Window::MakeTitle(char const * text, void const * ptr)
{
	static char buffer[256];
	snprintf(buffer, 256, "%s##%p", text, ptr);
	return buffer;
}


void Window::AddSource(Source * source)
{
	if(source == nullptr)
		throw std::invalid_argument("source must not be NULL!");
	if(source->window != nullptr)
		throw std::invalid_argument("source must not have a window assigned already!");
	source->window = this;
	source->index = this->sources.size();
	this->sources.emplace_back(source);
}

void Window::AddSink(Sink * sink)
{
	if(sink == nullptr)
		throw std::invalid_argument("source must not be NULL!");
	if(sink->window != nullptr)
		throw std::invalid_argument("source must not have a window assigned already!");
	sink->window = this;
	sink->index = this->sinks.size();
	this->sinks.emplace_back(sink);
}

nlohmann::json Window::Serialize() const
{
	return nlohmann::json();
}

void Window::Deserialize(nlohmann::json const & value)
{
	fprintf(stderr, "Deserialize: %s\n", value.dump().c_str());
}

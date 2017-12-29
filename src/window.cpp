#include "window.hpp"
#include <vector>
#include <algorithm>
#include <memory>
#include <cstdio>

#include "resources.hpp"

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

static ImTextureID GetSocketIcon(CgDataType type, bool isSource)
{
	switch(type)
	{
		case CgDataType::Geometry: return (ImTextureID)uintptr_t(resources::icons::geometry);
		case CgDataType::Shader: return (ImTextureID)uintptr_t(resources::icons::shader);
		case CgDataType::Texture2D: return (ImTextureID)uintptr_t(resources::icons::image);
		case CgDataType::UniformFloat: return (ImTextureID)uintptr_t(resources::icons::scalar);
		case CgDataType::UniformVec2: return (ImTextureID)uintptr_t(resources::icons::vector);
		case CgDataType::UniformVec3: return (ImTextureID)uintptr_t(resources::icons::vector);
		case CgDataType::UniformVec4: return (ImTextureID)uintptr_t(resources::icons::vector);
		case CgDataType::UniformMat3: return (ImTextureID)uintptr_t(resources::icons::matrix);
		case CgDataType::UniformMat4: return (ImTextureID)uintptr_t(resources::icons::matrix);
		default:
			if(isSource)
				return (ImTextureID)uintptr_t(resources::icons::genericSource);
			else
				return (ImTextureID)uintptr_t(resources::icons::genericSink);
	}
}

void Window::UpdateNodes()
{
	static Source * currentSource = nullptr;
	static Sink * currentSink = nullptr;

	Sink * hoveredSink = nullptr;
	Source * hoveredSource = nullptr;

	int offset;
	int margin = 4;
	int size = 24;

	auto * draw = ImGui::GetWindowDrawList();
	auto & io = ImGui::GetIO();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));

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
			ImGui::PushID(sink.get());
			if(ImGui::ImageButton(GetSocketIcon(sink->GetType(), false) , ImVec2(size, size)))
				sink->SetSource(nullptr);
			ImGui::PopID();
			if(currentSource != nullptr)
			{
				ImGui::PopStyleColor();
			}
			if(ImGui::IsItemHovered())
			{
				hoveredSink = sink.get();
				ImGui::SetTooltip("%s : %s", sink->GetName().c_str(), DisplayName(sink->GetType()));
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
			ImGui::PushID(source.get());
			ImGui::ImageButton(GetSocketIcon(source->GetType(), true), ImVec2(size, size));
			ImGui::PopID();
			if(ImGui::IsItemHovered())
			{
				hoveredSource = source.get();
				ImGui::BeginTooltip();

				ImGui::Text("%s : %s", source->GetName().c_str(), DisplayName(source->GetType()));

				DisplayDataValue(source->GetType(), source->GetObject());

				ImGui::EndTooltip();
			}
			if(ImGui::IsItemActive())
			{
				currentSource = source.get();
				draw->PushClipRectFullScreen();
				draw->AddLine(
					ImGui::CalcItemRectClosestPoint(io.MousePos, true, -2.0f),
					io.MousePos,
					ImColor(0.7f,0.7f,0.7f,1.0f),
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
	for(auto const & win : windows)
	{
		for(auto const & sink : win->sinks)
		{
			if(sink->GetSource(false) != nullptr)
			{
				ImVec2 pos(
					win->pos.x - size - margin,
					win->pos.y + margin + sink->GetWindowIndex() * (margin + size));

				auto * src = sink->GetSource();
				auto * srcwin = src->GetWindow();
				auto srcpos = srcwin->GetPosition();
				auto srcsize = srcwin->GetSize();
				int index = src->GetWindowIndex();

				ImVec2 from(srcpos.x + srcsize.x + margin + size, srcpos.y + margin + (size + margin) * index + size / 2);
				ImVec2 to(pos.x, pos.y + size / 2);

				bool selected = (src == hoveredSource || sink.get() == hoveredSink);
				draw->AddBezierCurve(
					from,
				    ImVec2(from.x + 64, from.y),
					ImVec2(to.x - 64, to.y),
					to,
					selected ? ImColor(1.0f, 0.4f, 0.4f, 1.0f) : ImColor(0.7f,0.7f,0.7f,1.0f),
					selected ? 4.0f : 2.0f);
			}
		}
	}
	ImGui::PopStyleVar();
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
	strcpy(this->titleEditBuffer, this->title.c_str());
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

	ImGui::PushID(this);

	char namebuf[512];
	snprintf(namebuf, 512, "%s##%p", this->title.c_str(), this);

	if(ImGui::Begin(namebuf, &this->isOpen, this->flags))
	{
		this->OnUpdate();

		if(ImGui::BeginPopupContextWindow())
		{
			ImGui::InputText("Node Name", this->titleEditBuffer, 256, ImGuiInputTextFlags_AutoSelectAll);

			if(ImGui::Button("Rename"))
			{
				this->title = std::string(this->titleEditBuffer);
				this->wantsResize = true;
			}


			ImGui::EndPopup();
		}

		this->pos = ImGui::GetWindowPos();
		this->size = ImGui::GetWindowSize();
	}
	ImGui::End();
	ImGui::PopID();
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

void Window::RemoveSource(Source * source, bool free)
{
	auto end = std::remove_if(this->sources.begin(), this->sources.end(), [source](std::unique_ptr<Source> const & ptr)
	{
		return (ptr.get() == source);
	});
	if(!free)
	{
		for(auto it = end; it != this->sources.end(); it++)
			it->release();
	}
	this->sources.erase(end, this->sources.end());

	for(size_t i = 0; i < this->sources.size(); i++)
		this->sources[i]->index = int(i);
}

void Window::RemoveSink(Sink * sink, bool free)
{
	auto end = std::remove_if(this->sinks.begin(), this->sinks.end(), [sink](std::unique_ptr<Sink> const & ptr)
	{
		return (ptr.get() == sink);
	});
	if(!free)
	{
		for(auto it = end; it != this->sinks.end(); it++)
			it->release();
	}
	this->sinks.erase(end, this->sinks.end());

	for(size_t i = 0; i < this->sinks.size(); i++)
		this->sinks[i]->index = int(i);
}

nlohmann::json Window::Serialize() const
{
	return nlohmann::json();
}

void Window::Deserialize(nlohmann::json const & value)
{
	fprintf(stderr, "Deserialize: %s\n", value.dump().c_str());
}

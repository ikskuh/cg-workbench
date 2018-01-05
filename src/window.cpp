#include "window.hpp"
#include <vector>
#include <algorithm>
#include <memory>
#include <cstdio>
#include <windowregistry.hpp>

#include "resources.hpp"
#include "fileio.hpp"
#include <fstream>

static std::vector<std::unique_ptr<Window>> windows;

int currentWindowID = 0;

std::vector<std::unique_ptr<Window>>::iterator Window::Begin() { return windows.begin(); }
std::vector<std::unique_ptr<Window>>::iterator Window::End()   { return windows.end(); }

static std::vector<std::unique_ptr<Window>> new_windows;

void Window::Register(Window * window)
{
	assert(window != nullptr);

	// Use the second vector to provide a safe method to duplicate windows:
	// Don't change a vector when iterating it!
	new_windows.emplace_back(window);
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
	// HACK: Required for allowing window creation while iteration
	for(auto & win : new_windows)
	{
		Window * window = win.release();
		auto pos = std::find_if(windows.begin(), windows.end(), [window](std::unique_ptr<Window> const & p) {
			return (p.get() == window);
		});
		if(pos == windows.end())
			windows.emplace_back(window);
	}
	new_windows.clear();

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
		case CgDataType::Audio: return (ImTextureID)uintptr_t(resources::icons::audio);
        case CgDataType::RenderPass: return (ImTextureID)uintptr_t(resources::icons::render_pass);
		case CgDataType::Event:
			if(isSource)
				return (ImTextureID)uintptr_t(resources::icons::event_source);
			else
				return (ImTextureID)uintptr_t(resources::icons::event_listener);
		case CgDataType::UniformFloat: return (ImTextureID)uintptr_t(resources::icons::scalar);
		case CgDataType::UniformVec2: return (ImTextureID)uintptr_t(resources::icons::vector2);
		case CgDataType::UniformVec3: return (ImTextureID)uintptr_t(resources::icons::vector3);
		case CgDataType::UniformVec4: return (ImTextureID)uintptr_t(resources::icons::vector4);
		case CgDataType::UniformMat3: return (ImTextureID)uintptr_t(resources::icons::matrix);
		case CgDataType::UniformMat4: return (ImTextureID)uintptr_t(resources::icons::matrix);
		default:
			if(isSource)
				return (ImTextureID)uintptr_t(resources::icons::genericSource);
			else
				return (ImTextureID)uintptr_t(resources::icons::genericSink);
	}
}

extern ImFont * labelFont;

void Window::UpdateNodes()
{
	static Source * currentSource = nullptr;
	static Sink * currentSink = nullptr;

	static Sink * hoveredSink = nullptr;
	static Source * hoveredSource = nullptr;

	int offset;
	int margin = 4;
	int size = 24;

	auto * draw = ImGui::GetWindowDrawList();
	auto & io = ImGui::GetIO();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));

	ImVec4 slotBaseColor(0.3f, 0.3f, 0.3f, 1.0f);
	ImVec4 linkBaseColor(0.3f, 0.3f, 0.3f, 0.8f);
	ImVec4 linkHoverColor(0.8f, 0.3f, 0.3f, 1.0f);

	float labelSize = labelFont->FontSize;
	ImColor labelColor(1.0f, 1.0f, 1.0f, 0.9f);

	currentSink = nullptr;

	for(auto const & win : windows)
	{
		for(auto const & sink : win->sinks)
		{
			ImVec2 pos(
				win->pos.x - size - margin,
				win->pos.y + margin + sink->GetWindowIndex() * (margin + size));

			for(int i = 0; i < sink->GetSourceCount(); i++)
			{
				auto * src = sink->GetSource(false, i);
				auto * srcwin = src->GetWindow();
				auto srcpos = srcwin->GetPosition();
				auto srcsize = srcwin->GetSize();
				int index = src->GetWindowIndex();

				ImVec2 from(srcpos.x + srcsize.x + margin + size, srcpos.y + margin + (size + margin) * index + size / 2);
				ImVec2 to(pos.x, pos.y + size / 2);

				bool selected = (src == hoveredSource || sink.get() == hoveredSink);

				float width = 2.0;
				ImColor color(linkBaseColor);

				if(selected)
				{
					color = linkHoverColor;
					width = 4.0;
				}

				if(src->GetType() == CgDataType::Event)
				{
					float str = 0.3 - glm::clamp<float>(
						src->GetObject<CgDataType::Event>().GetTimeSinceLastTrigger(),
						0.0,
						0.3);
					// TODO: Make better animation here!
					width += 4.0 * str;
					color.Value.x += str;
					color.Value.y += str;
					color.Value.z += str;
				}

				draw->AddBezierCurve(
					from,
				    ImVec2(from.x + 64, from.y),
					ImVec2(to.x - 64, to.y),
					to,
					color,
					width);
			}
		}
	}

	hoveredSink = nullptr;
	hoveredSource = nullptr;

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

			ImVec4 color(slotBaseColor);

			float str = 0.0;
			for(int i = 0; i < sink->GetSourceCount(); i++)
			{
				if(sink->GetSource(false, i)->GetType() == CgDataType::Event)
				{
					str = std::max<float>(str, 0.3 - glm::clamp<float>(
						sink->GetObject<CgDataType::Event>(i).GetTimeSinceLastTrigger(),
						0.0,
						0.3));
				}
			}

			color.x += str;
			color.y += str;
			color.z += str;

			if(currentSource != nullptr)
			{
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
			}
			ImGui::PushStyleColor(ImGuiCol_Button, color);
			ImGui::PushID(sink.get());
            if(ImGui::ImageButton(GetSocketIcon(sink->GetType(), false) , ImVec2(size, size), ImVec2(0,1), ImVec2(1,0)))
				sink->Clear();
			ImGui::PopID();
			ImGui::PopStyleColor();

			ImVec2 label = ImGui::GetFont()->CalcTextSizeA(
				labelSize,
				1000.0f,
				1000.0f,
				sink->GetName().c_str());

			draw->AddText(
				labelFont,
				labelSize,
				ImVec2(pos.x - margin - label.x, pos.y + (size - labelSize) / 2),
				labelColor,
				sink->GetName().c_str());

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
			ImVec2 pos(
				win->pos.x + win->size.x + margin,
				win->pos.y + offset);
			ImGui::SetCursorScreenPos(pos);
			ImVec4 color(slotBaseColor);

			if(source->GetType() == CgDataType::Event)
			{
				float str = 0.3 - glm::clamp<float>(
					source->GetObject<CgDataType::Event>().GetTimeSinceLastTrigger(),
					0.0,
					0.3);
				color.x += str;
				color.y += str;
				color.z += str;
			}

			ImGui::PushStyleColor(ImGuiCol_Button, color);
			ImGui::PushID(source.get());
            ImGui::ImageButton(GetSocketIcon(source->GetType(), true), ImVec2(size, size), ImVec2(0,1), ImVec2(1,0));
			ImGui::PopID();
			ImGui::PopStyleColor();

			draw->AddText(
				labelFont,
				labelSize,
				ImVec2(pos.x + margin + size, pos.y + (size - labelSize) / 2),
				labelColor,
				source->GetName().c_str());

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
					currentSink->AddSource(currentSource);
				currentSource = nullptr;
			}
			offset += size + margin;
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
		if(win.get() == nullptr) // this is ourselves, as we are deleted right now
			continue;
		if(win.get() == this) // this is weird
			continue;

		for(auto const & source : this->sources)
		{
			for(auto const & sink : win->sinks)
				sink->RemoveSource(source.get());
		}
	}
}

void Window::Update()
{
	for(auto & ev : this->events)
		ev->triggered = false;

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
			if(ImGui::InputText("Node Name", this->titleEditBuffer, 256, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			{
				this->title = std::string(this->titleEditBuffer);
				this->wantsResize = true;
			}

			ImGui::Separator();

			if(ImGui::MenuItem("Export Node..."))
			{
				auto path = FileIO::SaveDialog("jnode");
				if(!path.empty())
				{
					auto obj = this->Serialize();
					obj["window-type"] = this->GetTypeID();
					obj["window-title"] = this->GetTitle();

					std::ofstream stream(path);
					stream << obj;
					stream.flush();
				}
			}

			if(ImGui::MenuItem("Import Node..."))
			{
				auto path = FileIO::OpenDialog("jnode");
				if(!path.empty())
				{
					nlohmann::json data;

					std::ifstream stream(path);
					stream >> data;

					if(data["window-type"] == this->GetTypeID())
					{
						this->Deserialize(data);
					}
				}
			}

			ImGui::Separator();

			if(ImGui::MenuItem("Duplicate"))
			{
				auto obj = this->Serialize();
				obj["window-type"] = this->GetTypeID();
				obj["window-title"] = this->GetTitle();

				auto * win = Window::CreateFromJSON(obj);

				if(win)
				{
					win->pos = this->dupSpawnPos;
					win->size = this->size;
					win->wantsResize = true;

					Window::Register(win);
				}
			}
			else
			{
				this->dupSpawnPos = ImGui::GetIO().MousePos;
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
	return nlohmann::json::object();
}

void Window::Deserialize(nlohmann::json const & value)
{
	fprintf(stderr, "Deserialize: %s\n", value.dump().c_str());
}


Source * Window::GetSource(std::string name)
{
	for(auto const & src : this->sources)
		if(src->GetName() == name)
			return src.get();
	return nullptr;
}

Source const * Window::GetSource(std::string name) const
{
	for(auto const & src : this->sources)
		if(src->GetName() == name)
			return src.get();
	return nullptr;
}

Sink * Window::GetSink(std::string name)
{
	for(auto const & src : this->sinks)
		if(src->GetName() == name)
			return src.get();
	return nullptr;
}

Sink const * Window::GetSink(std::string name) const
{
	for(auto const & src : this->sinks)
		if(src->GetName() == name)
			return src.get();
	return nullptr;
}


Event * Window::CreateEvent()
{
	Event * ev;
	this->events.emplace_back(ev = new Event());
	return ev;
}

Window * Window::CreateFromJSON(nlohmann::json const & window)
{
	if(window.is_null())
		return nullptr;

	Window * win = nullptr;
	nlohmann::json type = window["window-type"];

	for(WindowClass * cl = WindowClass::First(); cl != nullptr; cl = cl->Next())
	{
		if(type != cl->GetID())
			continue;
		win = cl->CreateInstance();
		break;
	}
	if(win == nullptr)
		return nullptr;

	win->Deserialize(window);
	win->title = window.value("window-title", win->title);

	return win;
}

#include "windows/audio/audionode.hpp"

sample_t * audio_destbuffer;

void Window::RenderAudio(void*  userdata, Uint8* stream, int len)
{
	(void)userdata;

	sample_t * samples = reinterpret_cast<sample_t*>(stream);
	len /= sizeof(sample_t);

	assert(len == (audio_buffersize * audio_channels));

	audio_destbuffer = samples;

	memset(audio_destbuffer, 0, sizeof(sample_t) * audio_buffersize * audio_channels);

	for(auto & win : ::windows)
	{
		auto * node = dynamic_cast<AudioNode*>(win.get());
		if(node == nullptr)
			continue;
		node->RenderAudio();
	}

	audio_destbuffer = nullptr;
}

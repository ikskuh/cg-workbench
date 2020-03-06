#include "trackernode.hpp"

#include "bpmnode.hpp"

#include <SDL.h>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(TrackerNode, Menu::Event, "event-tracker", "Tracker")

TrackerNode::TrackerNode() :
    Window("Tracker", ImGuiWindowFlags_AlwaysAutoResize),
    position(0)
{
	for(size_t i = 0; i < channel_count; i++)
	{
		this->outputs[i] = this->CreateEvent();
		this->AddSource<CgDataType::Event>("Channel " + std::to_string(i), this->outputs[i]);
	}

	this->reset = this->AddSink<CgDataType::Event>("Reset", -1);
	this->advance = this->AddSink<CgDataType::Event>("Advance", -1);
}

void TrackerNode::DoTracker()
{
	if(track.size() == 0)
		return;

	if(Event::Any(reset))
		position = 0;

	if(Event::Any(advance))
	{
		position = (position + 1) % track.size();

		for(size_t i = 0; i < channel_count; i++)
		{
			if(track[position].channels[i])
				outputs[i]->Trigger();
		}
	}
}

void TrackerNode::OnSetup()
{
	ImGui::SetNextWindowSize(ImVec2(256, 0));
}

void TrackerNode::OnRender()
{
}

void TrackerNode::OnUpdate()
{
	DoTracker();

	ImGui::Columns(channel_count + 2, "tracker-table"); // 4-ways, with border
  ImGui::Separator();
	ImGui::Text("#"); ImGui::NextColumn();
	for(size_t i = 1; i <= channel_count; i++)
  {
		ImGui::Text("%lu", i); ImGui::NextColumn();
	}
	ImGui::Text(" "); ImGui::NextColumn();
  ImGui::Separator();

	int index = 0;
	for(auto it = track.begin(); it != track.end();)
	{
		ImGui::PushID(&*it);

		ImGui::Selectable(std::to_string(index).c_str(), (index == position));
		index += 1;

		ImGui::NextColumn();

		for(size_t chan = 0; chan < it->channels.size(); chan++)
		{
			ImGui::PushID(chan);

      ImGui::Checkbox("", &it->channels[chan]);
			ImGui::NextColumn();

			ImGui::PopID();
		}

		if(ImGui::Button("X"))
			it = track.erase(it);
		else
			it ++;

		ImGui::NextColumn();
		ImGui::PopID();
  }
	for(int chan = 0; chan < channel_count + 1; chan++)
		ImGui::NextColumn();

	if(ImGui::Button("+"))
		track.emplace_back();
	ImGui::NextColumn();

  ImGui::Columns(1);
  ImGui::Separator();
}

WINDOW_SERIALIZE_IMPL(TrackerNode)
{

	nlohmann::json array;
	for(size_t i = 0; i < track.size(); i++)
	{
		nlohmann::json element;
		for(size_t j = 0; j < channel_count; j++)
		{
			element[j] = track[i].channels[j];
		}
		array.push_back(element);
	}

	nlohmann::json result;
	result["length"] = track.size();
	result["data"] = array;
	return result;
}

WINDOW_DESERIALIZE_IMPL(TrackerNode)
{
	track.clear();
	size_t length = data["length"];
	for(size_t i = 0; i < length; i++)
	{
		auto element = data["data"][i];

		auto & p = track.emplace_back();
		for(size_t j = 0; j < channel_count; j++)
		{
			p.channels[j] = element[j];
		}
	}
}

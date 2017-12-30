#pragma once

#include "window.hpp"

#include <SDL.h>
#include <algorithm>

template<CgDataType _Type>
class BufferWindow : public Window
{
	typedef typename UniformType<_Type>::type data_t;
private:
	std::vector<std::pair<uint32_t,data_t>> samples;
	float bufferLength;
	bool useSeconds;
	bool useSmoothing;
	data_t value;
	Sink * input;
protected:
	void OnRender() override
	{
		if(this->useSeconds)
		{
			Uint32 history = SDL_GetTicks() - int(1000 * this->bufferLength);
			while(this->samples.size() > 0 && this->samples.front().first < history)
				this->samples.erase(this->samples.begin());
		}
		else
		{
			while(this->samples.size() > ceil(this->bufferLength - 1.0))
				this->samples.erase(this->samples.begin());
		}

		this->samples.emplace_back(SDL_GetTicks(), this->input->GetObject<_Type>());

		if(this->useSmoothing)
		{
			data_t sum = data_t();
			for(auto const & val : this->samples)
				sum += val.second;
			this->value = sum / float(this->samples.size());
		}
		else
		{
			this->value = this->samples.front().second;
		}
	}

	void OnUpdate() override
	{
		ImGui::Text("Buffer Type:");
		if(ImGui::RadioButton("Time", this->useSeconds)) this->useSeconds = true;
		if(ImGui::RadioButton("Frame", !this->useSeconds)) this->useSeconds = false;

		ImGui::Checkbox("Smooth Output", &this->useSmoothing);

		ImGui::DragFloat(
			(this->useSeconds ? "Seconds" : "Frames"),
			&this->bufferLength,
			0.01f);
	}
public:
	BufferWindow() :
	    Window("Buffer", ImGuiWindowFlags_AlwaysAutoResize),
	    samples(),
	    bufferLength(1.0f),
	    useSeconds(true),
	    useSmoothing(false),
	    value()
	{
		this->AddSink(this->input = new Sink(_Type, "Input"));
		this->AddSource(new Source(_Type, "Output", &this->value));
	}

	nlohmann::json Serialize() const override
	{
		return {
			{ "type", std::string("buffer-") + DisplayName(_Type) },
			{ "length", this->bufferLength },
			{ "useTime", this->useSeconds },
			{ "smooth", this->useSmoothing },
		};
	}

	void Deserialize(const nlohmann::json &value) override
	{
		this->bufferLength = value["length"];
		this->useSeconds = value["useTime"];
		this->useSmoothing = value["smooth"];
	}
};

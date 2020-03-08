#include "linearnoisenode.hpp"
#include <stb_perlin.h>
#include "time.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(LinearNoiseNode, Menu::Noise, "noise-linear", "Linear Noise")

LinearNoiseNode::LinearNoiseNode() :
    Window("Linear Noise", ImGuiWindowFlags_AlwaysAutoResize),
    mode(0),
    freq(10),
    time(0.0),
    value(0.0)
{
	this->AddSource(new Source(CgDataType::UniformFloat, "Noise", &this->value));
}

void LinearNoiseNode::OnRender()
{
	switch(this->mode)
	{
		case 0:
			this->value = rand() / float(RAND_MAX);
			break;
		case 1:
			this->value = stb_perlin_noise3(this->time, 0, 0, 0, 0, 0);
			this->time += this->freq * Time::getVideoDelta();
			break;
	}
}

void LinearNoiseNode::OnUpdate()
{
	if(ImGui::RadioButton("White Noise", (this->mode == 0))) this->mode = 0;
	if(ImGui::RadioButton("1D Perlin Noise", (this->mode == 1))) this->mode = 1;

	if(this->mode > 0)
		ImGui::DragFloat("Frequency", &this->freq, 0.1f, 0.0f, 1000.0f);
}

nlohmann::json LinearNoiseNode::Serialize() const
{
	return {
		{ "freq", this->freq },
		{ "mode", this->mode }
	};
}

void LinearNoiseNode::Deserialize(const nlohmann::json &value)
{
	this->freq = value.value("freq", 10);
	this->mode = value.value("mode", 0);
}

#include "waveformviewer.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(WaveformViewer, Menu::Audio, "audio-waveform-viewer", "Waveform Viewer")

WaveformViewer::WaveformViewer() :
    AudioNode("Waveform Viewer"),
    input(nullptr),
    data(1),
    takeSample(false)
{
	this->input = this->AddSink<CgDataType::Audio>("Source");

}

void WaveformViewer::OnRenderAudio()
{
	if(this->takeSample == false)
		return;
	AudioStream const & stream = this->input->GetObject<CgDataType::Audio>();
	stream.CopyTo(this->data);
	this->takeSample = false;
}

void WaveformViewer::OnUpdate()
{
	this->takeSample |= ImGui::Button("Snapshot!");

	auto size = ImGui::GetContentRegionAvail();

	ImGui::PlotLines(
		"Waveform",
		this->data.GetData(),
		this->data.GetLength(),
		0,
		nullptr,
		-1.3f,
		1.3f,
		size);
}

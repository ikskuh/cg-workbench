#include "graphwindow.hpp"
#include <algorithm>

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(GraphWindow, Menu, "graph", "Graph")

GraphWindow::GraphWindow() :
    Window("Graph", 0),
    samples()
{
	this->AddSink(this->input = new Sink(CgDataType::UniformFloat, "Input"));
}

void GraphWindow::OnRender()
{
	this->samples.emplace_back(this->input->GetObject<CgDataType::UniformFloat>());
}

void GraphWindow::OnUpdate()
{
	ImVec2 size = ImGui::GetContentRegionAvail();
	if(size.x > 0 && this->samples.size() > size.x)
	{
		this->samples.erase(
			this->samples.begin(),
			this->samples.begin() + (this->samples.size() - int(size.x)));
	}
	if(this->samples.size() == 0)
		return;

	ImGui::PlotLines(
		"Values",
		this->samples.data(),
		this->samples.size(),
		0,
		nullptr,
		FLT_MAX,
		FLT_MAX,
		size);
}

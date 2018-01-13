#include "edgedetector.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(EdgeDetector, Menu::Event, "event-edge-detector", "Edge Detector")

EdgeDetector::EdgeDetector() :
    Window("Edge Detector", ImGuiWindowFlags_AlwaysAutoResize),
    threshold(0.5)
{
	this->value = this->AddSink<CgDataType::UniformFloat>("Value");

	this->transition = this->AddEventSource("Any Edge");
	this->positiveEdge = this->AddEventSource("Rising Edge");
	this->negativeEdge = this->AddEventSource("Falling Edge");
}

void EdgeDetector::OnRender()
{
	bool a = (this->value->GetObject<CgDataType::UniformFloat>() > this->threshold);
	if(a != this->above)
	{
		if(a)
			this->positiveEdge->Trigger();
		else
			this->negativeEdge->Trigger();
		this->transition->Trigger();
	}
	this->above = a;
}

void EdgeDetector::OnUpdate()
{
	ImGui::DragFloat("Threshold", &this->threshold, 0.1f);
}

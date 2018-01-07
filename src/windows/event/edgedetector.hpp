#pragma once

#include <window.hpp>

class EdgeDetector :
	public Window
{
	WINDOW_PREAMBLE
private:
	float threshold;
	Sink * value;
	Event * transition;
	Event * positiveEdge;
	Event * negativeEdge;
	bool above;
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	EdgeDetector();
};

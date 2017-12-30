#pragma once

#include "window.hpp"

class GraphWindow : public Window
{
	WINDOW_PREAMBLE
private:
	std::vector<float> samples;
	Sink * input;
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	GraphWindow();
};

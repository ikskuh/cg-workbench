#pragma once

#include <window.hpp>

class PulseNode :
	public Window
{
	WINDOW_PREAMBLE
private:
	Sink * trigger;
	float output;

protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	PulseNode();
};

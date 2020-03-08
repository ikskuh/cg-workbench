#pragma once

#include "window.hpp"

class BPMNode :
	public Window
{
	WINDOW_PREAMBLE
	WINDOW_CUSTOM_SERIALIZE
private:
	Event * tick;
	Sink * start;
	Sink * stop;
	bool ticking;
	int frequency;
	float timer;

	int startupDelay;
	float lastTaps[4];
protected:
	void OnUpdate() override;
public:
	BPMNode();
};

#pragma once

#include "window.hpp"

class BPMNode :
	public Window
{
	WINDOW_PREAMBLE
private:
	Event * tick;
	Sink * start;
	Sink * stop;
	bool ticking;
	int frequency;
	int timer;

	int lastTaps[4];
protected:
	void OnUpdate() override;
public:
	BPMNode();
};

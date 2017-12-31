#pragma once

#include "window.hpp"

#include <queue>

class EventDelay :
	public Window
{
	WINDOW_PREAMBLE
private:
	Sink * input;
	Event * output;
	std::queue<timestamp_t> delayedEvents;
	int delay;
protected:
	void OnUpdate();
public:
	EventDelay();
};

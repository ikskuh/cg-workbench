#pragma once

#include <window.hpp>

class EventCounter : public Window
{
	WINDOW_PREAMBLE
private:
	Sink * up;
	Sink * down;
	Sink * reset;
	float counter;
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	EventCounter();
};

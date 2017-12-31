#pragma once

#include "window.hpp"

class Trigger :
	public Window
{
	WINDOW_PREAMBLE
private:
	float value;
	bool triggered;
protected:
	void OnUpdate() override;
public:
	Trigger();
};

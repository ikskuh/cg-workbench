#pragma once

#include "../window.hpp"

class Trigger :
	public Window
{
private:
	float value;
protected:
	void OnUpdate() override;
public:
	Trigger();

	TRIVIAL_SERIALIZE("trigger")
};

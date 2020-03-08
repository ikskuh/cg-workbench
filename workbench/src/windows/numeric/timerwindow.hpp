#pragma once

#include "window.hpp"

#include <SDL.h>

class TimerWindow :
	public Window
{
	WINDOW_PREAMBLE
private:
	float total;
	float delta;
	float speed;
	float stamp;
	Sink * reset;
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	TimerWindow();
	~TimerWindow();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

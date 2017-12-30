#pragma once

#include "window.hpp"

#include <SDL.h>

class TimerWindow :
	public Window
{
private:
	float total;
	float delta;
	float speed;
	Uint32 stamp;
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	TimerWindow();
	~TimerWindow();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

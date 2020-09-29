#pragma once

#include "window.hpp"

class LinearNoiseNode : public Window
{
	WINDOW_PREAMBLE
private:
	int mode;
	float freq;
	float time;
	float value;

protected:
	void OnRender() override;
	void OnUpdate() override;

public:
	LinearNoiseNode();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

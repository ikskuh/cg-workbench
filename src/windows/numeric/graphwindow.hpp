#pragma once

#include "window.hpp"

class GraphWindow : public Window
{
private:
	std::vector<float> samples;
	Sink * input;
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	GraphWindow();

	nlohmann::json Serialize() const;

	void Deserialize(const nlohmann::json &value);
};

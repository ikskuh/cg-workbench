#pragma once

#include "../window.hpp"

class ColorWindow : public Window
{
private:
	glm::vec4 color;
protected:
	void OnUpdate() override;
public:
	ColorWindow();
	~ColorWindow();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

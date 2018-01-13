#pragma once

#include "window.hpp"

class VectorSplitter :
	public Window
{
	WINDOW_PREAMBLE
private:
	glm::vec4 output;
	Sink * input;
	int components;

	void SetComponents(int num);
protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	VectorSplitter();

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

class VectorMerger :
	public Window
{
	WINDOW_PREAMBLE
private:
	glm::vec4 output;
	Sink * inputX;
	Sink * inputY;
	Sink * inputZ;
	Sink * inputW;
protected:
	void OnRender() override;
	void OnUpdate() override { }
public:
	VectorMerger();
};

#pragma once

#include "window.hpp"

template<CgDataType _Type>
class UniformWindow :
	public Window
{
	WINDOW_PREAMBLE
private:
	typename UniformType<_Type>::type value;
	static char const * GetTitle();
protected:
	virtual void OnUpdate() override;
public:
	UniformWindow() :
	    Window(GetTitle(), ImGuiWindowFlags_AlwaysAutoResize)
	{
		this->AddSource(new Source(_Type, "Value", &this->value));
	}

	~UniformWindow()
	{

	}

	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

#pragma once

#include "window.hpp"

class NoteWindow :
	public Window
{
private:
	static const int MaxLength = 4096;
	char buffer[MaxLength];
protected:
	void OnUpdate() override;
public:
	NoteWindow();

	nlohmann::json Serialize() const;

	void Deserialize(const nlohmann::json &value);

};

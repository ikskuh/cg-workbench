#pragma once

#include "cgdatatype.hpp"
#include <string>

// Required for "compatibility" with function names.
#ifdef WIN32
#include <windows.h>
#endif

class Window;

class Slot
{
	friend class Window;

private:
	CgDataType type;
	std::string name;
	Window *window;
	int index;

protected:
	Slot(CgDataType type, std::string const &name);
	Slot(Slot const &) = delete;
	Slot(Slot &&) = delete;

public:
	virtual ~Slot();

	CgDataType GetType() const { return this->type; }

	std::string GetName() const { return this->name; }

	Window *GetWindow() const { return this->window; }

	int GetWindowIndex() const { return this->index; }
};

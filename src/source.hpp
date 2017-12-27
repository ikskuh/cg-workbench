#pragma once

#include "slot.hpp"
#include <GL/gl3w.h>

class Source :
	public Slot
{
private:
	GLuint const * object;
public:
	Source(CgDataType type, std::string const & name, GLuint const * object);
	virtual ~Source();

	GLuint const & GetObject() const { return *this->object; }

	static Source const * GetDefault(CgDataType type);
};

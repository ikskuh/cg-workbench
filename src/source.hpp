#pragma once

#include "slot.hpp"
#include <GL/gl3w.h>

class Source :
	public Slot
{
private:
	void const * object;
public:
	Source(CgDataType type, std::string const & name, void const * object);
	virtual ~Source();

	void const * GetObject() const { return this->object; }

	template<CgDataType _Type>
	typename UniformType<_Type>::type GetObject() const
	{
		return *static_cast<typename UniformType<_Type>::type const *>(this->object);
	}

	static Source const * GetDefault(CgDataType type);
};

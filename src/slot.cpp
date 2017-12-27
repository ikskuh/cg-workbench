#include "slot.hpp"

Slot::Slot(CgDataType type, std::string const & name) :
	type(type),
	name(name),
	window(nullptr),
    index(-1)
{

}

Slot::~Slot()
{

}

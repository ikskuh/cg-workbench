#include "sink.hpp"
#include <stdexcept>

Sink::Sink(CgDataType type, std::string const & name) :
	Slot(type, name),
    source(nullptr)
{

}

Sink::~Sink()
{

}

void Sink::SetSource(Source const * source)
{
	if(source != nullptr && source->GetType() != this->GetType())
		throw std::invalid_argument("Source must have correct type!");
	this->source = source;
}

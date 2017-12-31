#include "sink.hpp"
#include <stdexcept>
#include <algorithm>

Sink::Sink(CgDataType type, std::string const & name, int maxConnections) :
	Slot(type, name),
    sources(),
	limit(maxConnections)
{
	assert(this->limit == -1 || this->limit >= 1);
}

Sink::~Sink()
{

}

/*
void Sink::SetSource(Source const * source)
{
	if(source != nullptr && source->GetType() != this->GetType())
		throw std::invalid_argument("Source must have correct type!");
	this->source = source;
}
*/

void Sink::Clear()
{
	this->sources.clear();
}

bool Sink::AddSource(Source const * source)
{
	assert(source != nullptr && source->GetType() == this->GetType());
	if(this->limit == 1)
	{
		this->sources.clear();
		this->sources.push_back(source);
		return true;
	}
	if(this->limit >= 1 && int(this->sources.size()) >= this->limit)
		return false;
	auto it = std::find(this->sources.begin(), this->sources.end(), source);
	if(it != this->sources.end())
		return false;
	this->sources.push_back(source);
	return true;
}

void Sink::RemoveSource(Source const * source)
{
	this->sources.erase(std::remove(this->sources.begin(), this->sources.end(), source), this->sources.end());
}

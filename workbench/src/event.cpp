#include "event.hpp"
#include <SDL.h>

#include "sink.hpp"

timestamp_t Event::system_time;

Event::Event() :
    current(system_time - 10000), // 10 seconds ago...
    counter(0)
{

}

void Event::Trigger()
{
	this->triggered = true;
	this->current = system_time;
	this->counter += 1;
}

void Event::Reset()
{
	this->counter = 0;
}

float Event::GetTimeSinceLastTrigger() const
{
	return (system_time	- this->current) / 1000.0;
}



void Event::NewFrame()
{
	system_time = SDL_GetTicks();
}

bool Event::Any(Sink * sink)
{
	if(sink == nullptr)
		return false;
	assert(sink->GetType() == CgDataType::Event);
	for(int i = 0; i < sink->GetSourceCount(); i++)
	{
		if(sink->GetObject<CgDataType::Event>(i))
			return true;
	}
	return false;
}

int Event::Count(Sink * sink)
{
	if(sink == nullptr)
		return false;
	assert(sink->GetType() == CgDataType::Event);
	int cnt = 0;
	for(int i = 0; i < sink->GetSourceCount(); i++)
	{
		if(sink->GetObject<CgDataType::Event>(i))
			++cnt;
	}
	return cnt;
}

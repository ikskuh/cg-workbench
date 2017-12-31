#include "event.hpp"
#include <SDL.h>

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

void Event::NewFrame()
{
	system_time = SDL_GetTicks();
}

float Event::GetTimeSinceLastTrigger() const
{
	return (system_time	- this->current) / 1000.0;
}

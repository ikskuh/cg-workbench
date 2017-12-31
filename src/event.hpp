#pragma once

#include <stdint.h>

typedef uint32_t timestamp_t;

class Event
{
	friend class Window; // will reset the event
private:
	static timestamp_t system_time;
public:
	static void NewFrame();

private:
	timestamp_t current;
	bool triggered;
	int counter;
public:
	Event();

	void Trigger();

	//! Resets the event counter
	void Reset();

	//! Gets the time since the last event in seconds.
	float GetTimeSinceLastTrigger() const;

	int GetCounter() const { return this->counter; }

	bool IsActive() const { return this->triggered; }

	operator bool() const { return this->IsActive(); }
};


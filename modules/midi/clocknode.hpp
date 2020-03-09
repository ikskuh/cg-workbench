#ifndef CLOCKNODE_HPP
#define CLOCKNODE_HPP

#include <window.hpp>

#include <atomic>
#include "midi.hpp"

class ClockNode :
    public Window
{
    WINDOW_PREAMBLE
private:
    MIDI::Handle midiCallbackID;
    std::atomic_flag had_clock4_event;
    std::atomic_flag had_clock8_event;
    std::atomic_flag had_clock16_event;
    Event * beat4;
    Event * beat8;
    Event * beat16;
    uint32_t currentTicks;
protected:
	void OnUpdate() override;
public:
    ClockNode();
    ~ClockNode() override;
};

#endif // CLOCKNODE_HPP

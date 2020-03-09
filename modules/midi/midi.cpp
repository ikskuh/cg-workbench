#include "midi.hpp"

#include "RtMidi.h"

#include <memory>
#include <mutex>
#include <map>

static std::mutex instance_lock;
static std::unique_ptr<MIDI> instance;

struct CallbackStore
{
	uint8_t mask;
	MIDI::Callback callback;
};

static std::mutex callbackMutex;

static std::map<MIDI::Handle, CallbackStore> callbacks;

MIDI & MIDI::get()
{
    std::lock_guard guard { instance_lock };
    if(not instance) {
        instance = std::unique_ptr<MIDI>(new MIDI());
    }
    return *instance.get();
}

static void midiCallback( double timeStamp, std::vector<unsigned char> * message, void * /*userData*/)
{
	MIDI::Message msg;
	msg.event = message->at(0) & 0xF0;
	msg.channel = message->at(0) & 0x0F;
	msg.timestamp = timeStamp;
	std::copy(
		message->begin() + 1, message->end(),
		std::back_inserter(msg.payload));

	std::lock_guard<std::mutex> _lock(callbackMutex);

	for(auto const & _cb : callbacks)
    {
		auto const & cb = _cb.second;
		if((cb.mask == 0x00) || (cb.mask == msg.event))
			cb.callback(msg);
	}
}

MIDI::Handle MIDI::addCallback(uint8_t event, const MIDI::Callback &fun)
{
    std::lock_guard _lock(callbackMutex);
	static Handle maxHandle = 0;

	Handle h = ++maxHandle;
	callbacks.emplace(h, CallbackStore { event, fun });
	return h;
}

void MIDI::removeCallback(MIDI::Handle h)
{
    std::lock_guard _lock(callbackMutex);
	callbacks.erase(h);
}

static std::unique_ptr<RtMidiIn> midi;

MIDI::MIDI()
{
    midi = std::make_unique<RtMidiIn>(RtMidi::UNSPECIFIED, "CG Workbench");
	midi->setCallback(midiCallback, this);

//    Log() << "Available midi ports:";
//    unsigned int const midiPortCount = midi->getPortCount();
//    for(unsigned int i = 0; i < midiPortCount; i++)
//    {
//        Log() << "[" << i << "] " << midi->getPortName(i);
//    }

    // Windows does not support virtual midi ports by-api
#ifndef CGPLAT_WINDOWS
    // if(Utils::get(config, "useVirtualMidi", true))
    // {
        midi->openVirtualPort("Generic Input");
    // }
    // else
#else
    // {
        midi->openPort(Utils::get(config, "midiPort", 0), "Visualization Input");
    // }
#error "Windows is not supported yet!"
#endif
	midi->ignoreTypes(true, false, true); // no sysex, use time, no sense
}

MIDI::~MIDI()
{
    midi.reset();
}

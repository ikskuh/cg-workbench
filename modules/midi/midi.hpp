#ifndef MIDI_HPP
#define MIDI_HPP

#include <cstdint>
#include <vector>
#include <functional>

struct MIDI
{
    static MIDI & get();

public:
    static constexpr uint8_t NoteOff          = 0x80;
	static constexpr uint8_t NoteOn           = 0x90;
	static constexpr uint8_t Aftertouch       = 0xA0;
	static constexpr uint8_t ControlChange    = 0xB0;
	static constexpr uint8_t ProgramChange    = 0xC0;
	static constexpr uint8_t ChannelPressure  = 0xD0;
	static constexpr uint8_t PitchWheelChange = 0xE0;
    static constexpr uint8_t SysEx            = 0xF0;

	using Handle = uint32_t;

	struct Message
	{
		double timestamp;
		uint8_t event;
		uint8_t channel;
		std::vector<uint8_t> payload;
	};

	using Callback = std::function<void(Message const &)>;

	Handle addCallback(uint8_t event, Callback const & fun);

	void removeCallback(Handle h);

private:
    MIDI();
public:
    ~MIDI();
};

/*
1000nnnn	0kkkkkkk	0vvvvvvv	Note Off event.
									This message is sent when a note is released (ended).
									(kkkkkkk) is the key (note) number.
									(vvvvvvv) is the velocity.
1001nnnn	0kkkkkkk	0vvvvvvv	Note On event.
									This message is sent when a note is depressed (start).
									(kkkkkkk) is the key (note) number.
									(vvvvvvv) is the velocity.
1010nnnn	0kkkkkkk	0vvvvvvv	Polyphonic Key Pressure (Aftertouch).
									This message is most often sent by pressing down on the key after it "bottoms out".
									(kkkkkkk) is the key (note) number.
									(vvvvvvv) is the pressure value.
1011nnnn	0ccccccc	0vvvvvvv	Control Change.
									This message is sent when a controller value changes. Controllers include devices such as pedals and levers. Certain controller numbers are reserved for specific purposes. See Channel Mode Messages.
									(ccccccc) is the controller number.
									(vvvvvvv) is the new value.
1100nnnn	0ppppppp				Program Change.
									This message sent when the patch number changes.
									(ppppppp) is the new program number.
1101nnnn	0vvvvvvv				Channel Pressure (After-touch).
									This message is most often sent by pressing down on the key after it "bottoms out". This message is different from polyphonic after-touch. Use this message to send the single greatest pressure value (of all the current depressed keys).
									(vvvvvvv) is the pressure value.
1110nnnn	0lllllll	0mmmmmmm	Pitch Wheel Change.
									This message is sent to indicate a change in the pitch wheel. The pitch wheel is measured by a fourteen bit value. Centre (no pitch change) is 2000H. Sensitivity is a function of the transmitter.
									(lllllll) are the least significant 7 bits.
									(mmmmmmm) are the most significant 7 bits.
*/



#endif // MIDI_HPP

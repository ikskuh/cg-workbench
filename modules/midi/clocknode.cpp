#include "clocknode.hpp"

#include "windowregistry.hpp"
REGISTER_WINDOW_CLASS(ClockNode, Menu::Audio::Midi, "midi-clock-node", "Clock")

#include "time.hpp"

#include <iostream>

ClockNode::ClockNode() :
    Window("Clock", ImGuiWindowFlags_AlwaysAutoResize),
    currentTicks(0)
{
    this->midiCallbackID = MIDI::get().addCallback(
        MIDI::SysEx,
        [this](MIDI::Message const & msg) {

            int mask = 0x00;
            if(msg.channel == 0xA) { // Start
                this->currentTicks = 0;
                mask = 0xFF; // All notes are triggered
            } else if(msg.channel == 0x8) { // Clock / Pulse
                this->currentTicks += 1;

                if(0 == (this->currentTicks % 24)) mask |= 4;
                if(0 == (this->currentTicks % 12)) mask |= 8;
                if(0 == (this->currentTicks % 6)) mask |= 16;

                if(this->currentTicks >= 24) {
                    this->currentTicks = 0;
               }
            }

            if(mask & 16) this->had_clock16_event.clear();
            if(mask & 8) this->had_clock8_event.clear();
            if(mask & 4) this->had_clock4_event.clear();
        }
    );

    // Set to "no beat" state
    this->had_clock4_event.test_and_set();
    this->had_clock8_event.test_and_set();
    this->had_clock16_event.test_and_set();

    this->beat4 = this->CreateEvent();
    this->beat8 = this->CreateEvent();
    this->beat16 = this->CreateEvent();
	this->AddSource<CgDataType::Event>("1/4", this->beat4); // ♩
    this->AddSource<CgDataType::Event>("1/8", this->beat8); // ♪
    this->AddSource<CgDataType::Event>("1/16",  this->beat16); // ♬
}

ClockNode::~ClockNode()
{
    MIDI::get().removeCallback(this->midiCallbackID);
}

void ClockNode::OnUpdate()
{
    if(not this->had_clock4_event.test_and_set()) this->beat4->Trigger();
    if(not this->had_clock8_event.test_and_set()) this->beat8->Trigger();
    if(not this->had_clock16_event.test_and_set()) this->beat16->Trigger();

    ImGui::TextUnformatted("MIDI Clock");
}

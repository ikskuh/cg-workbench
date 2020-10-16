#include "time.hpp"
#include "audiostream.hpp"

#include <cstdint>
#include <atomic>

namespace
{
// these values store times in "audiosamples" for maximum precision
    std::atomic<uint64_t> current_time;

    uint64_t deltaV, lastVideoFrame;
}

void Time::init()
{
    current_time = 0;
    lastVideoFrame = 0;
}

float Time::get()
{
    return float(current_time.load(std::memory_order_acquire)) / float(audio_samplerate);
}

float Time::getAudioDelta()
{
    return float(audio_samplerate) / float(audio_samplerate);
}

float Time::getVideoDelta()
{
    return float(deltaV) / float(audio_samplerate);
}

void Time::newAudioFrame()
{
    (void)current_time.fetch_add(audio_buffersize);
}

void Time::newVideoFrame()
{
    uint64_t now = current_time.load(std::memory_order_acquire);
    deltaV = now - lastVideoFrame;
    lastVideoFrame = now;
}

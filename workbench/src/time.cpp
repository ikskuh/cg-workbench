#include "time.hpp"
#include "audiostream.hpp"

#include <cstdint>
#include <atomic>

namespace
{
// these values store times in "audiosamples" for maximum precision
    std::atomic<uint64_t> time;

    uint64_t deltaV, lastVideoFrame;
}

void Time::init()
{
    time = 0;
    lastVideoFrame = 0;
}

float Time::get()
{
    return float(time.load(std::memory_order_acquire)) / float(audio_samplerate);
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
    (void)time.fetch_add(audio_buffersize);
}

void Time::newVideoFrame()
{
    deltaV = time.load(std::memory_order_acquire) - lastVideoFrame;
    lastVideoFrame = time;
}

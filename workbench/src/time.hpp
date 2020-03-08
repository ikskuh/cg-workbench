#ifndef TIME_HPP
#define TIME_HPP

namespace Time
{
    void init();

    void newAudioFrame();

    void newVideoFrame();

    float get();

    float getAudioDelta();

    float getVideoDelta();
}

#endif // TIME_HPP

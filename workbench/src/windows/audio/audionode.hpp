#pragma once

#include "window.hpp"
#include "audiostream.hpp"

#include <functional>

class AudioNode :
	public Window
{
public:
	typedef std::function<void(sample_t * dst, int dstCnt, sample_t const * src, int srcCnt)> dsp_t;
	typedef std::function<sample_t(sample_t src)> monodsp_t;
private:
	int priority;

protected:
	AudioNode(std::string const & title, ImGuiWindowFlags flags = 0);

	//! Applies a processing to all audio samples, requires channel adjustments!
	static void Render(AudioStream & dst, AudioStream const & src, dsp_t const & dsp);

	//! Applies a processing to all channels, dst will have adjusted channel
	static void Render(AudioStream & dst, AudioStream const & src, monodsp_t const & dsp);

	//! Renders the audio stream
	//! @remarks Called from a different thread!
	virtual void OnRenderAudio() = 0;

public:
	virtual ~AudioNode();

	void RenderAudio();
};

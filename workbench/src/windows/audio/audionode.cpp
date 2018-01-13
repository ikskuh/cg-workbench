#include "audionode.hpp"

AudioNode::AudioNode(std::string const & title, ImGuiWindowFlags flags) :
    Window(title, flags),
    priority(-1)
{

}

AudioNode::~AudioNode()
{

}

void AudioNode::Render(AudioStream & dst, AudioStream const & src, dsp_t const & dsp)
{
	assert(dst.GetLength() == src.GetLength());
	int schan = src.GetChannels();
	int dchan = dst.GetChannels();
	for(int i = 0; i < src.GetLength(); i++)
	{
		dsp(&dst.Sample(i, 0), dchan, &src.Sample(i, 0), schan);
	}
}

//! Applies a processing to all channels, dst will have adjusted channel
void AudioNode::Render(AudioStream & dst, AudioStream const & src, monodsp_t const & dsp)
{
	assert(dst.GetLength() == src.GetLength());
	dst.SetFormatForStream(src);
	for(int i = 0; i < src.GetLength() * src.GetChannels(); i++)
	{
		dst[i] = dsp(src[i]);
	}
}


void AudioNode::RenderAudio()
{
	this->OnRenderAudio();
}

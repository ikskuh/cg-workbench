#ifndef TRACKERNODE_HPP
#define TRACKERNODE_HPP

#include "window.hpp"

class TrackerNode :
	public Window
{
	WINDOW_PREAMBLE
	WINDOW_CUSTOM_SERIALIZE
private:
	size_t static constexpr channel_count = 8;

	Sink * reset;
	Sink * advance;

	std::array<Event*, channel_count> outputs;

	struct Channel
	{
		std::array<bool, TrackerNode::channel_count> channels;
	};

	std::vector<Channel> track;

	size_t position;

	void DoTracker();

protected:
	void OnSetup() override;
	void OnRender() override;
	void OnUpdate() override;
public:
	TrackerNode();
};


#endif // TRACKERNODE_HPP

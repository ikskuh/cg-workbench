#ifndef IOMODULE_HPP
#define IOMODULE_HPP


#include <window.hpp>

class IoModule :
	public Window
{
	WINDOW_PREAMBLE
private:
    int serialPort;
    float analogOut[8];
    float digitalOut[4];
    Event * digitalEv[4];

    std::vector<char> readBuffer;

protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	IoModule();
    ~IoModule() override;
};

#endif // IOMODULE_HPP

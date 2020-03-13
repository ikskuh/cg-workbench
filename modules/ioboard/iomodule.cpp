#include "iomodule.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(IoModule, Menu, "iomodule-input", "IO Module")

#include <charconv>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static int set_interface_attribs (int fd, int speed, int parity)
{
    struct termios tty;
    if (tcgetattr (fd, &tty) != 0)
    {
        fprintf (stderr, "error %d from tcgetattr", errno);
        return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        fprintf (stderr, "error %d from tcsetattr", errno);
        return -1;
    }
    return 0;
}

void set_blocking (int fd, int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
        fprintf (stderr,"error %d from tggetattr", errno);
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
        fprintf (stderr,"error %d setting term attributes", errno);
}


IoModule::IoModule() :
    Window("IO Module", ImGuiWindowFlags_AlwaysAutoResize),
    serialPort(-1)
{
    for(int i = 0; i < 4; i++)
    {
        this->digitalEv[i] = this->CreateEvent();
        this->AddSource<CgDataType::Event>("Event " + std::to_string(i), this->digitalEv[i]);
    }
    for(int i = 0; i < 4; i++)
    {
        this->digitalOut[i] = 0.0f;
        this->AddSource<CgDataType::UniformFloat>("Digital " + std::to_string(i), &this->digitalOut[i]);
    }
    for(int i = 0; i < 8; i++)
    {
        this->analogOut[i] = 0.0f;
        this->AddSource<CgDataType::UniformFloat>("Analog " + std::to_string(i), &this->analogOut[i]);
    }


    char const * portname = "/dev/ttyUSB0";

    serialPort = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (serialPort < 0)
    {
        fprintf (stderr, "error %d opening %s: %s", errno, portname, strerror (errno));
        return;
    }

    set_interface_attribs (serialPort, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking (serialPort, 0);                // set no blocking

    fprintf(stderr, "%d\n", serialPort);
}

IoModule::~IoModule()
{
    if(serialPort >= 0)
        close(serialPort);
}

void IoModule::OnRender()
{
    if(serialPort < 0)
        return;
    {
        char buf [1024];
        ssize_t n = read (serialPort, buf, sizeof buf);  // read up to 100 characters if ready to read
        if(n > 0) {
            auto const off = readBuffer.size();
            readBuffer.resize(off + n);
            memcpy(readBuffer.data() + off, buf, n);
        }
    }

    // Clear the buffer if there is more than 64 bytes
    // in it. The longest expected line is 9 byte long,
    // so the data is bogus anyways…
    if(readBuffer.size() > 4096) {
        readBuffer.clear();
    }

    size_t i = 0;
    while(i < readBuffer.size())
    {
        if(readBuffer.at(i) == '\n') {
            // parse data input here
            // Format is x:y:…\n with x=(A|D), y={0…9}
            if(i > 4 and readBuffer[1] == ':' and readBuffer[3] == ':') {
                int id = readBuffer[2] - '0';
                if(id >= 0 and id <= 9) {
                    int value;
                    auto const res = std::from_chars<int>(
                        readBuffer.data() + 4,
                        readBuffer.data() + readBuffer.size(),
                        value,
                        10);
                    if(res.ptr != readBuffer.data() + 4) {
                        switch(readBuffer[0])
                        {
                            case 'A': {
                                if(id >= 0 and id < 8)
                                    this->analogOut[id] =value / 1024.0f;
                                break;
                            }
                            case 'D': {
                                if(value != 0)
                                    fprintf(stderr, "id: %d\tvalue: %d\n", id, value);
                                fflush(stderr);
                                if(id >= 0 and id < 4) {
                                    if(value != 0) {
                                        if(this->digitalOut[id] <= 0.5f) {
                                        this->digitalEv[id]->Trigger();
                                        }
                                    }
                                    this->digitalOut[id] = value;
                                }
                                break;
                            }

                            default:
                                fprintf(stderr, "received invalid data from serial port: wrong mode\n");
                                break;
                        }
                    } else {
                        fprintf(stderr, "received invalid data from serial port: not a number\n");
                    }
                } else {
                    fprintf(stderr, "received invalid data from serial port: wrong id\n");
                }

            } else {
                fprintf(stderr, "received invalid data from serial port: not enough data\n");
            }
            // remove
            readBuffer.erase(readBuffer.begin(), readBuffer.begin() + i + 1);
            i = 0;
        } else {
            i += 1;
        }
    }
}

void IoModule::OnUpdate()
{
    if(serialPort < 0)
        ImGui::Text("Device not found!");
    else
        ImGui::Text("Connected");
}


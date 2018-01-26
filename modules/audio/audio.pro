TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../module.pri)

!windows: {
	PKGCONFIG += libpulse libpulse-simple
}

SOURCES += \
    audiooutput.cpp \
    mergechannelsnode.cpp \
    soundfile.cpp \
    splitchannelsnode.cpp \
    synthnode.cpp \
    waveformviewer.cpp \
    pulsemicrophone.cpp

HEADERS += \
    audiooutput.hpp \
    mergechannelsnode.hpp \
    soundfile.hpp \
    splitchannelsnode.hpp \
    synthnode.hpp \
    waveformviewer.hpp \
    pulsemicrophone.hpp

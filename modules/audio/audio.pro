TEMPLATE = lib
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../module.pri)

!windows: {
	PKGCONFIG += libpulse libpulse-simple
}

SOURCES += \
    ../../ext/kiss_fft/kiss_fft.c \
    ../../ext/kiss_fft/tools/kiss_fftr.c \
    audio2texture.cpp \
    audiooutput.cpp \
    envelopenode.cpp \
    mergechannelsnode.cpp \
    soundfile.cpp \
    splitchannelsnode.cpp \
    synthnode.cpp \
    waveformviewer.cpp \
    pulsemicrophone.cpp

HEADERS += \
    audio2texture.hpp \
    audiooutput.hpp \
    envelopenode.hpp \
    mergechannelsnode.hpp \
    soundfile.hpp \
    splitchannelsnode.hpp \
    synthnode.hpp \
    waveformviewer.hpp \
    pulsemicrophone.hpp

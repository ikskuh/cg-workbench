TEMPLATE = lib
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../module.pri)

!windows: {
	PKGCONFIG += libpulse libpulse-simple
}

windows: {
  DEFINES +=__WINDOWS_MM__
  LIBS += -lwinmm
}

!windows: {
  DEFINES += __LINUX_ALSA__
  LIBS += -lasound
  PACKAGES = alsa
}

SOURCES += \
        ../../ext/rtmidi/RtMidi.cpp \
        clocknode.cpp \
        midi.cpp

HEADERS += \
  ../../ext/rtmidi/RtMidi.h \
  clocknode.hpp \
  midi.hpp

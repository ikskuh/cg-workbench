TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../module.pri)

SOURCES += \
    audiooutput.cpp \
    mergechannelsnode.cpp \
    soundfile.cpp \
    splitchannelsnode.cpp \
    synthnode.cpp \
    waveformviewer.cpp

SUBDIRS += \
    audio.pro

HEADERS += \
    audiooutput.hpp \
    mergechannelsnode.hpp \
    soundfile.hpp \
    splitchannelsnode.hpp \
    synthnode.hpp \
    waveformviewer.hpp

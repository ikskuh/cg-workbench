TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../module.pri)

SOURCES += \
    ../../ext/webcam-v4l2/webcam.cpp \
    webcamwindow.cpp

HEADERS += \
    ../../ext/webcam-v4l2/webcam.h \
    webcamwindow.hpp

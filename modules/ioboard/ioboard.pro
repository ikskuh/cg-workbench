TEMPLATE = lib
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../module.pri)

HEADERS += \
  iomodule.hpp

SOURCES += \
  iomodule.cpp

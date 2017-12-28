TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig

PKGCONFIG += lua gl sdl2 SDL2_image

LIBS += -lm -ldl

INCLUDEPATH += ext/gl3w ext/stb ext/imgui ext/json ext/nativefiledialog/src/include

LIBS += $$PWD/ext/nativefiledialog/src/libnfd.a $$system(pkg-config --libs gtk+-3.0)

QMAKE_CFLAGS += $$system(pkg-config --cflags gtk+-3.0)
QMAKE_CXXFLAGS += $$system(pkg-config --cflags gtk+-3.0)

DEFINES += IMGUI_DISABLE_OBSOLETE_FUNCTIONS

SOURCES += \
	src/main.cpp \
	src/imgui_impl.cpp \
	ext/gl3w/gl3w.c \
	ext/imgui/imgui.cpp \
	ext/imgui/imgui_demo.cpp \
	ext/imgui/imgui_draw.cpp \
    src/window.cpp \
    src/windows/shadereditor.cpp \
    src/windows/luaconsole.cpp \
    src/windows/renderwindow.cpp \
    src/source.cpp \
    src/sink.cpp \
    src/slot.cpp \
    src/windows/geometrywindow.cpp \
    src/windows/gpuerrorlog.cpp \
    src/imageloader.cpp \
    src/resources.cpp \
    src/windows/uniformwindow.cpp \
    src/windows/colorwindow.cpp \
    src/windows/timerwindow.cpp \
    src/windows/arithmeticwindow.cpp \
    src/windows/notewindow.cpp \
    src/windows/bufferwindow.cpp \
    src/windows/graphwindow.cpp \
    src/windows/vectoradapter.cpp \
    src/windows/imagesource.cpp \
    src/windows/trigger.cpp \
    src/windows/imagebuffer.cpp \
    src/windows/matrixtransforms.cpp

HEADERS += \
	src/imgui_impl.h \
	ext/gl3w/GL/gl3w.h \
	ext/imgui/imgui.h \
	ext/imgui/imgui_internal.h \
	ext/json/json.hpp \
	ext/nativefiledialog/src/include/nfd.h \
    src/window.hpp \
    src/windows/shadereditor.hpp \
    src/windows/luaconsole.hpp \
    ext/imgui/imconfig.h \
    src/windows/renderwindow.hpp \
    src/source.hpp \
    src/sink.hpp \
    src/cgdatatype.hpp \
    src/slot.hpp \
    src/windows/geometrywindow.hpp \
    src/windows/gpuerrorlog.hpp \
    src/imageloader.hpp \
    src/resources.hpp \
    src/windows/uniformwindow.hpp \
    src/windows/colorwindow.hpp \
    src/windows/timerwindow.hpp \
    src/windows/arithmeticwindow.hpp \
    src/windows/notewindow.hpp \
    src/windows/bufferwindow.hpp \
    src/windows/graphwindow.hpp \
    src/windows/vectoradapter.hpp \
    src/windows/imagesource.hpp \
    src/windows/trigger.hpp \
    src/windows/imagebuffer.hpp \
    src/windows/matrixtransforms.hpp \
    src/windows/typemap.hpp

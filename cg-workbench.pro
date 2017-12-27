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
    src/windows/gpuerrorlog.cpp

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
    src/windows/gpuerrorlog.hpp

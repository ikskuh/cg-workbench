TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig

PKGCONFIG += lua gl sdl2 SDL2_image

LIBS += -lm -ldl

INCLUDEPATH += \
	$$PWD/src \
	$$PWD/ext/gl3w \
	$$PWD/ext/stb \
	$$PWD/ext/imgui \
	$$PWD/ext/json \
	$$PWD/ext/nativefiledialog/src/include \
	$$PWD/ext/tinyobjloader \
	$$PWD/ext/tinydir

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
    src/source.cpp \
    src/sink.cpp \
    src/slot.cpp \
    src/imageloader.cpp \
    src/resources.cpp \
    src/geometry.cpp \
    ext/tinyobjloader/tiny_obj_loader.cpp \
    src/textureeditor.cpp \
    src/window.cpp \
    src/windows/generic/luaconsole.cpp \
    src/windows/generic/notewindow.cpp \
    src/windows/generic/trigger.cpp \
    src/windows/graphic/imagebuffer.cpp \
    src/windows/graphic/imagesource.cpp \
    src/windows/graphic/shadereditor.cpp \
    src/windows/graphic/renderwindow.cpp \
    src/windows/graphic/geometrywindow.cpp \
    src/windows/graphic/gpuerrorlog.cpp \
    src/windows/numeric/uniformwindow.cpp \
    src/windows/numeric/colorwindow.cpp \
    src/windows/numeric/timerwindow.cpp \
    src/windows/numeric/arithmeticwindow.cpp \
    src/windows/numeric/bufferwindow.cpp \
    src/windows/numeric/graphwindow.cpp \
    src/windows/numeric/vectoradapter.cpp \
    src/windows/numeric/matrixtransforms.cpp \
    src/windowregistry.cpp \
    src/shaderprogram.cpp \
    src/fileio.cpp \
    src/windows/generic/linearnoisenode.cpp

HEADERS += \
	src/imgui_impl.h \
	ext/gl3w/GL/gl3w.h \
	ext/imgui/imgui.h \
	ext/imgui/imgui_internal.h \
	ext/json/json.hpp \
	ext/tinydir/tinydir.h \
	ext/nativefiledialog/src/include/nfd.h \
    src/window.hpp \
    ext/imgui/imconfig.h \
    src/source.hpp \
    src/sink.hpp \
    src/cgdatatype.hpp \
    src/slot.hpp \
    src/resources.hpp \
    src/imageloader.hpp \
    src/geometry.hpp \
    ext/tinyobjloader/tiny_obj_loader.h \
    src/textureeditor.hpp \
    src/windows/typemap.hpp \
    src/windows/generic/luaconsole.hpp \
    src/windows/generic/notewindow.hpp \
    src/windows/generic/trigger.hpp \
    src/windows/graphic/shadereditor.hpp \
    src/windows/graphic/renderwindow.hpp \
    src/windows/graphic/geometrywindow.hpp \
    src/windows/graphic/imagesource.hpp \
    src/windows/graphic/gpuerrorlog.hpp \
    src/windows/graphic/imagebuffer.hpp \
    src/windows/numeric/uniformwindow.hpp \
    src/windows/numeric/colorwindow.hpp \
    src/windows/numeric/timerwindow.hpp \
    src/windows/numeric/arithmeticwindow.hpp \
    src/windows/numeric/bufferwindow.hpp \
    src/windows/numeric/graphwindow.hpp \
    src/windows/numeric/vectoradapter.hpp \
    src/windows/numeric/matrixtransforms.hpp \
    src/windowregistry.hpp \
    src/shaderprogram.hpp \
    src/fileio.hpp \
    src/windows/generic/linearnoisenode.hpp

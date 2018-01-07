TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
	$$PWD/src \
	$$PWD/ext/gl3w \
	$$PWD/ext/stb \
	$$PWD/ext/imgui \
	$$PWD/ext/json \
	$$PWD/ext/nativefiledialog/src/include \
	$$PWD/ext/tinyobjloader \
	$$PWD/ext/tinydir

debug: {
    DEFINES += DEBUG_BUILD
}

windows: {

    # Make C++ on windows actually acceptable
    DEFINES += \
        NOMINMAX \
        WIN32_LEAN_AND_MEAN

    # TODO: Fix code for working with win32 wide chars.
    DEFINES -= UNICODE _UNICODE

    # Add local libraries here...
    INCLUDEPATH += \
        $$PWD/lib/SDL2-2.0.7/include \
        $$PWD/lib/glm \
        $$PWD/lib/lua-5.3.4/include

    LIBS += \
        $$PWD/lib/SDL2-2.0.7/lib/x64/SDL2.lib \
        $$PWD/lib/SDL2-2.0.7/lib/x64/SDL2main.lib \
        $$PWD/lib/lua-5.3.4/lua53.lib \
        -lOpenGL32 \
        -lole32 \
        -lShell32 \
        -lShlwapi

    debug {
        LIBS += $$PWD/lib/nfd/nfd_d.lib
    } else {
        LIBS += $$PWD/lib/nfd/nfd.lib
    }
}

!windows: {
    CONFIG += link_pkgconfig

    PKGCONFIG += lua gl sdl2 gtk+-3.0

    LIBS += -lm -ldl

    LIBS += $$PWD/ext/nativefiledialog/src/libnfd.a

    QMAKE_CFLAGS += $$system(pkg-config --cflags gtk+-3.0)
    QMAKE_CXXFLAGS += $$system(pkg-config --cflags gtk+-3.0)
}


DEFINES += \
	IMGUI_DISABLE_OBSOLETE_FUNCTIONS \
	STB_VORBIS_HEADER_ONLY

SOURCES += \
	ext/gl3w/gl3w.c \
	ext/imgui/imgui.cpp \
	ext/imgui/imgui_demo.cpp \
	ext/imgui/imgui_draw.cpp \
	src/imgui_impl.cpp \
	ext/tinyobjloader/tiny_obj_loader.cpp \
	src/audiostream.cpp \
	src/event.cpp \
	src/fileio.cpp \
	src/geometry.cpp \
	src/imageloader.cpp \
	src/resources.cpp \
	src/shaderprogram.cpp \
	src/sink.cpp \
	src/slot.cpp \
	src/source.cpp \
	src/textureeditor.cpp \
	src/window.cpp \
	src/windowregistry.cpp \
	src/windows/audio/audionode.cpp \
	src/windows/audio/audiooutput.cpp \
	src/windows/audio/mergechannelsnode.cpp \
	src/windows/audio/soundfile.cpp \
	src/windows/audio/splitchannelsnode.cpp \
	src/windows/audio/synthnode.cpp \
	src/windows/audio/waveformviewer.cpp \
	src/windows/event/bpmnode.cpp \
	src/windows/event/eventdelay.cpp \
	src/windows/event/trigger.cpp \
    src/windows/graphic/shadereditor.cpp \
    src/windows/generic/luaconsole.cpp \
    src/windows/graphic/gpuerrorlog.cpp \
    src/windows/generic/linearnoisenode.cpp \
    src/windows/generic/notewindow.cpp \
    src/windows/graphic/geometrywindow.cpp \
    src/windows/graphic/imagebuffer.cpp \
    src/windows/graphic/imagesource.cpp \
    src/windows/graphic/renderwindow.cpp \
    src/windows/numeric/arithmeticwindow.cpp \
    src/windows/numeric/bufferwindow.cpp \
    src/windows/numeric/colorwindow.cpp \
    src/windows/numeric/graphwindow.cpp \
    src/windows/numeric/matrixtransforms.cpp \
    src/windows/numeric/timerwindow.cpp \
    src/windows/numeric/uniformwindow.cpp \
    src/windows/numeric/vectoradapter.cpp \
	src/main.cpp \
    src/windows/graphic/renderpassnode.cpp \
    src/renderpass.cpp \
    src/utils.cpp \
    src/windows/event/eventcounter.cpp \
    src/windows/event/edgedetector.cpp \
    src/windows/event/pulsenode.cpp

HEADERS += \
	ext/gl3w/GL/gl3w.h \
	ext/imgui/imgui.h \
	ext/imgui/imgui_internal.h \
	ext/json/json.hpp \
	ext/nativefiledialog/src/include/nfd.h \
	ext/tinydir/tinydir.h \
	ext/imgui/imconfig.h \
	ext/tinyobjloader/tiny_obj_loader.h \
	src/audiostream.hpp \
	src/cgdatatype.hpp \
	src/event.hpp \
	src/fileio.hpp \
	src/geometry.hpp \
	src/imageloader.hpp \
	src/resources.hpp \
	src/shaderprogram.hpp \
	src/sink.hpp \
	src/slot.hpp \
	src/source.hpp \
	src/textureeditor.hpp \
	src/window.hpp \
	src/windowregistry.hpp \
	src/windows/audio/audionode.hpp \
	src/windows/audio/audiooutput.hpp \
	src/windows/audio/mergechannelsnode.hpp \
	src/windows/audio/soundfile.hpp \
	src/windows/audio/splitchannelsnode.hpp \
	src/windows/audio/synthnode.hpp \
	src/windows/audio/waveformviewer.hpp \
	src/windows/event/bpmnode.hpp \
	src/windows/event/eventdelay.hpp \
	src/windows/event/trigger.hpp \
    src/windows/graphic/shadereditor.hpp \
    src/windows/generic/luaconsole.hpp \
    src/windows/graphic/gpuerrorlog.hpp \
    src/windows/generic/linearnoisenode.hpp \
    src/windows/generic/notewindow.hpp \
    src/windows/graphic/geometrywindow.hpp \
    src/windows/graphic/imagebuffer.hpp \
    src/windows/graphic/imagesource.hpp \
    src/windows/graphic/renderwindow.hpp \
    src/windows/numeric/arithmeticwindow.hpp \
    src/windows/numeric/bufferwindow.hpp \
    src/windows/numeric/colorwindow.hpp \
    src/windows/numeric/graphwindow.hpp \
    src/windows/numeric/matrixtransforms.hpp \
    src/windows/numeric/timerwindow.hpp \
    src/windows/numeric/uniformwindow.hpp \
    src/windows/numeric/vectoradapter.hpp \
	src/imgui_impl.h \
    src/meshes/quad.h \
    src/windows/graphic/renderpassnode.hpp \
    src/renderpass.hpp \
    src/utils.hpp \
    src/windows/event/eventcounter.hpp \
    src/windows/event/edgedetector.hpp \
    src/windows/event/pulsenode.hpp

DISTFILES += \
    README.md \
    LICENSE



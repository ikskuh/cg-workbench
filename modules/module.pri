
include($$PWD/../ext/externals.pri)

INCLUDEPATH += \
	$$PWD/../workbench/src/

windows: {
	error("Not supported yet!")
}

!windows: {
	CONFIG += link_pkgconfig

	PKGCONFIG += lua gl sdl2 gtk+-3.0

	QMAKE_CFLAGS += $$system(pkg-config --cflags gtk+-3.0)
	QMAKE_CXXFLAGS += $$system(pkg-config --cflags gtk+-3.0)
}

# All modules go to the same folder
DESTDIR = $$BUILDROOT/plugins

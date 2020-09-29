const std = @import("std");

pub fn build(b: *std.build.Builder) !void {
    const target = b.standardTargetOptions(.{});

    const workbench = b.addExecutable("cg-workbench", null);
    workbench.linkLibC();
    workbench.linkSystemLibrary("c++");

    workbench.defineCMacro("GLM_ENABLE_EXPERIMENTAL");
    workbench.defineCMacro("DEBUG_BUILD");
    workbench.defineCMacro("IMGUI_DISABLE_OBSOLETE_FUNCTIONS");
    workbench.defineCMacro("STB_VORBIS_HEADER_ONLY");

    if (target.getOsTag() == .windows) {
        workbench.defineCMacro("NOMINMAX");
        workbench.defineCMacro("WIN32_LEAN_AND_MEAN");
        workbench.defineCMacro("CGPLAT_WINDOWS");
    } else {
        workbench.addLibPath("./ext/nativefiledialog/src/");
        workbench.linkSystemLibrary("nfd");

        workbench.linkSystemLibrary("m");
        workbench.linkSystemLibrary("dl");
        workbench.linkSystemLibrary("lua");
        workbench.linkSystemLibrary("gl");
        workbench.linkSystemLibrary("sdl2");
        workbench.linkSystemLibrary("gtk+-3.0");

        workbench.defineCMacro("CGPLAT_LINUX");
    }

    const cpp_options = &[_][]const u8{
        "-std=c++17",
        "-fno-sanitize=undefined",
    };
    const c_options = &[_][]const u8{
        "-fno-sanitize=undefined",
    };

    for (workbench_sources) |src| {
        workbench.addCSourceFile(
            src,
            if (std.mem.endsWith(u8, src, ".cpp")) cpp_options else c_options,
        );
    }
    for (include_dirs) |dir| {
        workbench.addIncludeDir(dir);
    }

    workbench.install();
}

const include_dirs = [_][]const u8{
    "./workbench/src",
    "./ext/gl3w",
    "./ext/stb",
    "./ext/imgui",
    "./ext/json",
    "./ext/nativefiledialog/src/include",
    "./ext/tinyobjloader",
    "./ext/tinydir",
    "./ext/webcam-v4l2",
    "./ext/kiss_fft",
    "./ext/kiss_fft/tools",
    "./ext/rtmidi",
};

const workbench_sources = [_][]const u8{
    "./ext/gl3w/gl3w.c",
    "./ext/imgui/imgui.cpp",
    "./ext/imgui/imgui_demo.cpp",
    "./ext/imgui/imgui_draw.cpp",
    "./ext/tinyobjloader/tiny_obj_loader.cpp",
    "./ext/imgui/imgui_widgets.cpp",
    "./workbench/src/cgdatatype.cpp",
    "./workbench/src/imgui_impl.cpp",
    "./workbench/src/audiostream.cpp",
    "./workbench/src/event.cpp",
    "./workbench/src/fileio.cpp",
    "./workbench/src/geometry.cpp",
    "./workbench/src/imageloader.cpp",
    "./workbench/src/resources.cpp",
    "./workbench/src/shaderprogram.cpp",
    "./workbench/src/sink.cpp",
    "./workbench/src/slot.cpp",
    "./workbench/src/source.cpp",
    "./workbench/src/textureeditor.cpp",
    "./workbench/src/time.cpp",
    "./workbench/src/window.cpp",
    "./workbench/src/windowregistry.cpp",
    "./workbench/src/windows/event/bpmnode.cpp",
    "./workbench/src/windows/event/eventdelay.cpp",
    "./workbench/src/windows/event/trackernode.cpp",
    "./workbench/src/windows/event/trigger.cpp",
    "./workbench/src/windows/graphic/shadereditor.cpp",
    "./workbench/src/windows/generic/luaconsole.cpp",
    "./workbench/src/windows/graphic/gpuerrorlog.cpp",
    "./workbench/src/windows/generic/linearnoisenode.cpp",
    "./workbench/src/windows/generic/notewindow.cpp",
    "./workbench/src/windows/graphic/geometrywindow.cpp",
    "./workbench/src/windows/graphic/imagebuffer.cpp",
    "./workbench/src/windows/graphic/imagesource.cpp",
    "./workbench/src/windows/graphic/renderwindow.cpp",
    "./workbench/src/windows/numeric/arithmeticwindow.cpp",
    "./workbench/src/windows/numeric/bufferwindow.cpp",
    "./workbench/src/windows/numeric/colorwindow.cpp",
    "./workbench/src/windows/numeric/graphwindow.cpp",
    "./workbench/src/windows/numeric/matrixtransforms.cpp",
    "./workbench/src/windows/numeric/timerwindow.cpp",
    "./workbench/src/windows/numeric/uniformwindow.cpp",
    "./workbench/src/windows/numeric/vectoradapter.cpp",
    "./workbench/src/main.cpp",
    "./workbench/src/windows/graphic/renderpassnode.cpp",
    "./workbench/src/renderpass.cpp",
    "./workbench/src/utils.cpp",
    "./workbench/src/windows/event/eventcounter.cpp",
    "./workbench/src/windows/event/edgedetector.cpp",
    "./workbench/src/windows/event/pulsenode.cpp",
    "./workbench/src/windows/graphic/noisetexture.cpp",
    "./workbench/src/audionode.cpp",
};

const std = @import("std");

const resources = struct {
    const @"/icons/geometry.png" = @embedFile("icons/geometry.png");
    const @"/icons/shader.png" = @embedFile("icons/shader.png");
    const @"/icons/image.png" = @embedFile("icons/image.png");
    const @"/icons/power-socket.png" = @embedFile("icons/power-socket.png");
    const @"/icons/power-plug.png" = @embedFile("icons/power-plug.png");
    const @"/icons/numeric.png" = @embedFile("icons/numeric.png");
    const @"/icons/vec2.png" = @embedFile("icons/vec2.png");
    const @"/icons/vec3.png" = @embedFile("icons/vec3.png");
    const @"/icons/vec4.png" = @embedFile("icons/vec4.png");
    const @"/icons/matrix.png" = @embedFile("icons/matrix.png");
    const @"/icons/audio.png" = @embedFile("icons/audio.png");
    const @"/icons/event_source.png" = @embedFile("icons/event_source.png");
    const @"/icons/event_listener.png" = @embedFile("icons/event_listener.png");
    const @"/icons/render_pass.png" = @embedFile("icons/render_pass.png");
};

pub const Resource = extern struct {
    file_name: [*:0]const u8,
    data: [*]const u8,
    length: usize,
};

fn make_zero_terminated(comptime str: []const u8) [:0]const u8 {
    comptime var buffer: [str.len + 1]u8 = undefined;
    buffer[0..str.len].* = str[0..str.len].*;
    buffer[str.len] = 0;
    return buffer[0..str.len :0];
}

export fn cgw_find_resource(c_name: [*:0]const u8, resource: *Resource) c_int {
    const name = std.mem.span(c_name);
    inline for (std.meta.declarations(resources)) |decl| {
        if (std.mem.eql(u8, name, decl.name)) {
            resource.* = Resource{
                .file_name = make_zero_terminated(decl.name).ptr,
                .data = @field(resources, decl.name),
                .length = @field(resources, decl.name).len,
            };
            return 1;
        }
    }
    return 0;
}

const std = @import("std");

comptime {
    _ = @import("resources.zig");
}

var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};

pub fn main() !u8 {
    const gpa = if (std.builtin.link_libc) std.heap.c_allocator else &general_purpose_allocator.allocator;
    defer if (!std.builtin.link_libc) {
        _ = general_purpose_allocator.deinit();
    };
    var arena_instance = std.heap.ArenaAllocator.init(gpa);
    defer arena_instance.deinit();
    const arena = &arena_instance.allocator;

    const args = try std.process.argsAlloc(arena);

    for (args) |arg, i| {
        std.debug.print("[{}] = {}\n", .{ i, arg });
    }

    const inital_file: ?[:0]const u8 = if (args.len > 1)
        try arena.dupeZ(u8, args[1])
    else
        null;

    return workbench_main(
        if (inital_file) |i| i.ptr else null,
    );
}

extern fn workbench_main(initial_file_name: ?[*:0]const u8) callconv(.C) u8;

const std = @import("std");

comptime {
    _ = @import("resources.zig");
}

var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};

pub fn main() void {
    // this is only here to make zig happy
    // we still use the main() from main.cpp
    unreachable;
}

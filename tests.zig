const std = @import("std");
const c = @import("c");

test "public header imports into Zig" {
    _ = c;
    try std.testing.expect(true);
}

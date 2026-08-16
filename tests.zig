const std = @import("std");
const c = @import("c");

fn testAlloc(_: ?*anyopaque, size: usize, _: usize) callconv(.c) ?*anyopaque {
    return std.c.malloc(size);
}

fn testFree(_: ?*anyopaque, ptr: ?*anyopaque, _: usize, _: usize) callconv(.c) void {
    std.c.free(ptr);
}

const test_vtable = c.allocator_vtable_t{
    .alloc = testAlloc,
    .free = testFree,
};

fn testAllocator() c.allocator_t {
    return .{ .vtable = &test_vtable };
}

test "parse i32 distinguishes valid, invalid, and overflow input" {
    const min = c.sigma_parse_i32("-2147483648");
    try std.testing.expectEqual(@as(c_uint, c.sigma_parse_i32_ok), min.tag);
    try std.testing.expectEqual(@as(i32, -2147483648), min.value);

    const invalid = c.sigma_parse_i32("12nope");
    try std.testing.expectEqual(@as(c_uint, c.sigma_parse_i32_invalid), invalid.tag);

    const overflow = c.sigma_parse_i32("2147483648");
    try std.testing.expectEqual(@as(c_uint, c.sigma_parse_i32_overflow), overflow.tag);
}

test "memory primitives handle exact byte ranges" {
    var source = [_]u8{ 1, 2, 3, 4, 5 };
    var destination = [_]u8{0} ** source.len;

    _ = c.ft_memcpy(&destination, &source, source.len);
    try std.testing.expectEqualSlices(u8, &source, &destination);

    _ = c.ft_memset(&destination[1], 0xaa, 3);
    try std.testing.expectEqualSlices(u8, &.{ 1, 0xaa, 0xaa, 0xaa, 5 }, &destination);
    try std.testing.expectEqual(@as(usize, 5), c.ft_strlen("hello"));
}

test "SIMD memory paths preserve guards and tails" {
    var source: [193]u8 = undefined;
    var destination = [_]u8{0xcc} ** 197;
    for (&source, 0..) |*byte, index|
        byte.* = @truncate(index * 17);

    _ = c.ft_memcpy(&destination[2], &source, source.len);
    try std.testing.expectEqual(@as(u8, 0xcc), destination[1]);
    try std.testing.expectEqualSlices(u8, &source, destination[2 .. 2 + source.len]);
    try std.testing.expectEqual(@as(u8, 0xcc), destination[195]);

    _ = c.ft_memset(&destination[3], 0x5a, 191);
    try std.testing.expectEqual(@as(u8, source[0]), destination[2]);
    try std.testing.expectEqualSlices(u8, &([_]u8{0x5a} ** 191), destination[3..194]);
    try std.testing.expectEqual(@as(u8, source[192]), destination[194]);
}

test "SIMD strlen handles unaligned long strings" {
    const text = "x" ++ ("0123456789abcdef" ** 8);
    try std.testing.expectEqual(@as(usize, text.len - 1), c.ft_strlen(text.ptr + 1));
}

test "string map grows and retrieves values" {
    var map: c.sigma_str_map = undefined;
    try std.testing.expect(c.sigma_str_map_init(&map, testAllocator(), 0));
    defer c.sigma_str_map_deinit(&map);

    const key = c.sigma_str_from_cstr("project");
    const value = c.sigma_str_from_cstr("sigma_libft");
    try std.testing.expect(c.sigma_str_map_put(&map, key, value));
    const found = c.sigma_str_map_get(&map, key);
    try std.testing.expect(found != null);
    try std.testing.expectEqualSlices(u8, "sigma_libft", found.*.items[0..found.*.len]);
}

test "line reader preserves buffered lines and reports eof" {
    var fds: [2]c_int = undefined;
    try std.testing.expectEqual(@as(c_int, 0), c.pipe(&fds));
    defer _ = c.close(fds[0]);

    const input = "first\nsecond\r\n";
    try std.testing.expectEqual(@as(isize, input.len), c.write(fds[1], input, input.len));
    _ = c.close(fds[1]);

    var reader: c.sigma_line_reader = undefined;
    c.sigma_line_reader_init(&reader, fds[0], testAllocator());
    defer c.sigma_line_reader_deinit(&reader);

    const first = c.sigma_line_reader_next(&reader);
    try std.testing.expectEqual(@as(c_uint, c.sigma_line_ok), first.tag);
    try std.testing.expectEqualSlices(u8, "first", first.line.items[0..first.line.len]);

    const second = c.sigma_line_reader_next(&reader);
    try std.testing.expectEqual(@as(c_uint, c.sigma_line_ok), second.tag);
    try std.testing.expectEqualSlices(u8, "second", second.line.items[0..second.line.len]);

    const eof = c.sigma_line_reader_next(&reader);
    try std.testing.expectEqual(@as(c_uint, c.sigma_line_eof), eof.tag);
}

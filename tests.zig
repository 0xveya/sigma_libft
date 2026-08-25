const std = @import("std");
const c = @import("c");

extern fn sigma_test_vec_codegen() bool;
extern fn sigma_test_string_ownership() bool;

var allocation_count: usize = 0;
var free_count: usize = 0;

fn testAlloc(_: ?*anyopaque, size: usize, _: usize) callconv(.c) ?*anyopaque {
    allocation_count += 1;
    return std.c.malloc(size);
}

fn testFree(_: ?*anyopaque, ptr: ?*anyopaque, _: usize, _: usize) callconv(.c) void {
    free_count += 1;
    std.c.free(ptr);
}

const test_vtable = c.allocator_vtable_t{
    .alloc = testAlloc,
    .free = testFree,
};

fn testAllocator() c.allocator_t {
    return .{ .vtable = &test_vtable };
}

fn cString(ptr: [*c]u8) []u8 {
    return std.mem.span(@as([*:0]u8, @ptrCast(ptr)));
}

fn noopDelete(_: ?*anyopaque) callconv(.c) void {}

var iter_count: usize = 0;
fn countNode(_: ?*anyopaque) callconv(.c) void {
    iter_count += 1;
}

fn identityContent(content: ?*anyopaque) callconv(.c) ?*anyopaque {
    return content;
}

test "vector codegen preserves ownership contracts" {
    try std.testing.expect(sigma_test_vec_codegen());
}

test "owned strings and string vectors preserve ownership contracts" {
    try std.testing.expect(sigma_test_string_ownership());
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

test "string map grows and retrieves values" {
    var map: c.sigma_str_map = undefined;
    try std.testing.expect(c.sigma_str_map_init(&map, testAllocator(), 0));
    defer c.sigma_str_map_deinit(&map);

    const key = c.str_from_cstr("project");
    const value = c.str_from_cstr("sigma_libft");
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

test "ASCII table classifies and converts every byte" {
    for (0..256) |value| {
        const byte: u8 = @intCast(value);
        const is_lower = byte >= 'a' and byte <= 'z';
        const is_upper = byte >= 'A' and byte <= 'Z';
        const is_digit = byte >= '0' and byte <= '9';
        const is_space = byte == ' ' or (byte >= '\t' and byte <= '\r');
        const is_cntrl = byte <= 0x1f or byte == 0x7f;
        const is_print = byte >= 0x20 and byte <= 0x7e;
        const is_xdigit = is_digit or
            (byte >= 'A' and byte <= 'F') or
            (byte >= 'a' and byte <= 'f');

        try std.testing.expectEqual(is_lower or is_upper, c.ascii_is_alpha(byte));
        try std.testing.expectEqual(is_digit, c.ascii_is_digit(byte));
        try std.testing.expectEqual(is_cntrl, c.ascii_is_cntrl(byte));
        try std.testing.expectEqual(is_lower or is_upper or is_digit, c.ascii_is_alnum(byte));
        try std.testing.expectEqual(is_lower, c.ascii_is_lower(byte));
        try std.testing.expectEqual(is_print, c.ascii_is_print(byte));
        try std.testing.expectEqual(is_upper, c.ascii_is_upper(byte));
        try std.testing.expectEqual(is_space, c.ascii_is_space(byte));
        try std.testing.expectEqual(is_xdigit, c.ascii_is_xdigit(byte));
        try std.testing.expectEqual(if (is_upper) byte + 32 else byte, c.ascii_to_lower(byte));
        try std.testing.expectEqual(if (is_lower) byte - 32 else byte, c.ascii_to_upper(byte));
    }
}

test "ft_atoi" {
    try std.testing.expectEqual(@as(c_int, -42), c.ft_atoi("-42"));
}

test "ft_itoa" {
    const value = c.ft_itoa(-2147483648);
    try std.testing.expect(value != null);
    defer std.c.free(value);
    try std.testing.expectEqualStrings("-2147483648", cString(value));
}

test "ft_bzero" {
    var bytes = [_]u8{ 1, 2, 3, 4 };
    c.ft_bzero(&bytes[1], 2);
    try std.testing.expectEqualSlices(u8, &.{ 1, 0, 0, 4 }, &bytes);
}

test "ft_calloc" {
    const ptr = c.ft_calloc(17, 1);
    try std.testing.expect(ptr != null);
    defer std.c.free(ptr);
    const bytes: [*]u8 = @ptrCast(ptr.?);
    try std.testing.expectEqualSlices(u8, &([_]u8{0} ** 17), bytes[0..17]);
}

test "ft_memchr" {
    const text = "abc";
    const found = c.ft_memchr(text, 'b', text.len);
    try std.testing.expect(found != null);
    try std.testing.expectEqual(@as(u8, 'b'), @as(*const u8, @ptrCast(found.?)).*);
}

test "ft_memcmp" {
    try std.testing.expect(c.ft_memcmp("abc", "abd", 3) < 0);
}

test "ft_memcpy" {
    var dst = [_]u8{0} ** 65;
    const src = [_]u8{0x7b} ** 65;
    try std.testing.expect(c.ft_memcpy(&dst, &src, src.len) != null);
    try std.testing.expectEqualSlices(u8, &src, &dst);
}

test "ft_memmove" {
    var bytes = [_:0]u8{ 'a', 'b', 'c', 'd' };
    _ = c.ft_memmove(&bytes[1], &bytes[0], 3);
    try std.testing.expectEqualSlices(u8, "aabc", bytes[0..4]);
}

test "ft_memset" {
    var bytes = [_]u8{0} ** 65;
    try std.testing.expect(c.ft_memset(&bytes, 0xa5, bytes.len) != null);
    try std.testing.expectEqualSlices(u8, &([_]u8{0xa5} ** 65), &bytes);
}

test "ft_lstnew" {
    var value: c_int = 42;
    const node = c.ft_lstnew(&value);
    try std.testing.expect(node != null);
    defer std.c.free(node);
    try std.testing.expect(node.*.content == @as(?*anyopaque, @ptrCast(&value)) and node.*.next == null);
}

test "ft_lstadd_front" {
    var first = c.t_list{ .content = null, .next = null };
    var front = c.t_list{ .content = null, .next = null };
    var list: [*c]c.t_list = &first;
    c.ft_lstadd_front(&list, &front);
    try std.testing.expect(list == &front and front.next == &first);
}

test "ft_lstadd_back" {
    var first = c.t_list{ .content = null, .next = null };
    var back = c.t_list{ .content = null, .next = null };
    var list: [*c]c.t_list = &first;
    c.ft_lstadd_back(&list, &back);
    try std.testing.expect(first.next == &back);
}

test "ft_lstlast" {
    var last = c.t_list{ .content = null, .next = null };
    var first = c.t_list{ .content = null, .next = &last };
    try std.testing.expect(c.ft_lstlast(&first) == &last);
}

test "ft_lstsize" {
    var last = c.t_list{ .content = null, .next = null };
    var first = c.t_list{ .content = null, .next = &last };
    try std.testing.expectEqual(@as(c_int, 2), c.ft_lstsize(&first));
}

test "ft_lstiter" {
    var last = c.t_list{ .content = null, .next = null };
    var first = c.t_list{ .content = null, .next = &last };
    iter_count = 0;
    c.ft_lstiter(&first, countNode);
    try std.testing.expectEqual(@as(usize, 2), iter_count);
}

test "ft_lstdelone" {
    const node = c.ft_lstnew(null);
    try std.testing.expect(node != null);
    c.ft_lstdelone(node, noopDelete);
}

test "ft_lstclear" {
    var list = c.ft_lstnew(null);
    try std.testing.expect(list != null);
    list.*.next = c.ft_lstnew(null);
    try std.testing.expect(list.*.next != null);
    c.ft_lstclear(&list, noopDelete);
    try std.testing.expect(list == null);
}

test "ft_lstmap" {
    var last = c.t_list{ .content = null, .next = null };
    var first = c.t_list{ .content = null, .next = &last };
    var mapped = c.ft_lstmap(&first, identityContent, noopDelete);
    try std.testing.expect(mapped != null);
    defer c.ft_lstclear(&mapped, noopDelete);
    try std.testing.expectEqual(@as(c_int, 2), c.ft_lstsize(mapped));
}

test "ft_fprintf" {
    var fds: [2]c_int = undefined;
    try std.testing.expectEqual(@as(c_int, 0), c.pipe(&fds));
    const written = c.ft_fprintf(fds[1], "value=%d %s", @as(c_int, 42), "ok");
    _ = c.close(fds[1]);
    var buffer: [64]u8 = undefined;
    const count = c.read(fds[0], &buffer, buffer.len);
    _ = c.close(fds[0]);
    try std.testing.expectEqual(@as(c_int, 11), written);
    try std.testing.expectEqualStrings("value=42 ok", buffer[0..@intCast(count)]);
}

test "ft_printf" {
    var fds: [2]c_int = undefined;
    try std.testing.expectEqual(@as(c_int, 0), c.pipe(&fds));
    const saved = c.dup(c.STDOUT_FILENO);
    try std.testing.expect(saved >= 0);
    try std.testing.expectEqual(c.STDOUT_FILENO, c.dup2(fds[1], c.STDOUT_FILENO));
    _ = c.close(fds[1]);
    const written = c.ft_printf("value=%d %s", @as(c_int, 42), "ok");
    try std.testing.expectEqual(c.STDOUT_FILENO, c.dup2(saved, c.STDOUT_FILENO));
    _ = c.close(saved);
    var buffer: [64]u8 = undefined;
    const count = c.read(fds[0], &buffer, buffer.len);
    _ = c.close(fds[0]);
    try std.testing.expectEqual(@as(c_int, 11), written);
    try std.testing.expectEqualStrings("value=42 ok", buffer[0..@intCast(count)]);
}

test "str converts from and to C strings" {
    const string = c.str_from_cstr("hello");
    try std.testing.expectEqualSlices(u8, "hello", string.items[0..string.len]);

    const bytes = [_]u8{ 'a', 0, 'b' };
    allocation_count = 0;
    const converted = c.str_to_cstr(.{ .items = &bytes, .len = bytes.len }, testAllocator());
    try std.testing.expect(converted != null);
    defer testFree(null, converted, bytes.len + 1, @alignOf(std.c.max_align_t));
    try std.testing.expectEqual(@as(usize, 1), allocation_count);
    try std.testing.expectEqualSlices(u8, &.{ 'a', 0, 'b', 0 }, converted[0 .. bytes.len + 1]);
}

test "str slices and compares bounded contents" {
    const string = c.str_from_cstr("alphabet");
    try std.testing.expect(!c.str_is_empty(string));
    try std.testing.expect(c.str_is_empty(c.str_sub(string, string.len, 10)));
    try std.testing.expectEqualSlices(u8, "pha", c.str_sub(string, 2, 3).items[0..3]);
    try std.testing.expectEqualSlices(u8, "bet", c.str_sub(string, 5, 99).items[0..3]);
    try std.testing.expect(c.str_eq(c.str_from_cstr("same"), c.str_from_cstr("same")));
    try std.testing.expect(!c.str_eq(c.str_from_cstr("same"), c.str_from_cstr("other")));
    try std.testing.expect(c.str_cmp(c.str_from_cstr("abc"), c.str_from_cstr("abd")) < 0);
    try std.testing.expect(c.str_cmp(c.str_from_cstr("abc"), c.str_from_cstr("ab")) > 0);
    try std.testing.expect(c.str_starts_with(string, c.str_from_cstr("alpha")));
    try std.testing.expect(c.str_ends_with(string, c.str_from_cstr("bet")));
}

test "str finds bytes and trims ASCII whitespace" {
    const string = c.str_from_cstr("abca");
    try std.testing.expectEqual(@as(usize, 0), c.str_find_byte(string, 'a'));
    try std.testing.expectEqual(@as(usize, 3), c.str_rfind_byte(string, 'a'));
    try std.testing.expectEqual(@as(usize, c.SIGMA_NPOS), c.str_find_byte(string, 'x'));

    const spaced = c.str_from_cstr(" \t hello \r\n");
    const start = c.str_trim_start_ascii(spaced);
    const end = c.str_trim_end_ascii(spaced);
    const both = c.str_trim_ascii(spaced);
    try std.testing.expectEqualSlices(u8, "hello \r\n", start.items[0..start.len]);
    try std.testing.expectEqualSlices(u8, " \t hello", end.items[0..end.len]);
    try std.testing.expectEqualSlices(u8, "hello", both.items[0..both.len]);
}

test "str scalar split preserves empty fields" {
    var iterator = c.str_split_scalar(c.str_from_cstr("a::b:"), ':');
    var part: c.str_t = undefined;
    const expected = [_][]const u8{ "a", "", "b", "" };

    for (expected) |value| {
        try std.testing.expect(c.str_split_scalar_next(&iterator, &part));
        try std.testing.expectEqualSlices(u8, value, part.items[0..part.len]);
    }
    try std.testing.expect(!c.str_split_scalar_next(&iterator, &part));
}

test "bytes slices immutable and mutable views" {
    const source = [_]u8{ 0, 1, 2, 3 };
    const bytes: c.bytes_t = .{ .items = &source, .len = source.len };
    try std.testing.expect(!c.bytes_is_empty(bytes));
    try std.testing.expect(c.bytes_is_empty(.{ .items = null, .len = 0 }));

    const middle = c.bytes_sub(bytes, 1, 2);
    try std.testing.expectEqualSlices(u8, &.{ 1, 2 }, middle.items[0..middle.len]);
    const clamped = c.bytes_sub(bytes, 99, 99);
    try std.testing.expectEqual(@as(usize, 0), clamped.len);

    const empty = c.bytes_sub(.{ .items = null, .len = 0 }, 0, 1);
    try std.testing.expectEqual(@as(usize, 0), empty.len);
    try std.testing.expect(empty.items == null);

    var mutable_source = [_]u8{ 4, 5, 6 };
    const mutable = c.bytes_mut_sub(.{ .items = &mutable_source, .len = mutable_source.len }, 1, 1);
    mutable.items[0] = 9;
    try std.testing.expectEqual(@as(u8, 9), mutable_source[1]);
}

test "bytes compares bounded contents" {
    const low = [_]u8{ 0, 0xff };
    const high = [_]u8{ 0, 0 };
    const low_bytes: c.bytes_t = .{ .items = &low, .len = low.len };
    const high_bytes: c.bytes_t = .{ .items = &high, .len = high.len };

    try std.testing.expect(c.bytes_eq(low_bytes, low_bytes));
    try std.testing.expect(!c.bytes_eq(low_bytes, high_bytes));
    try std.testing.expect(c.bytes_cmp(low_bytes, high_bytes) > 0);
    try std.testing.expect(c.bytes_cmp(.{ .items = null, .len = 0 }, .{ .items = null, .len = 0 }) == 0);
}

test "bytes finds values within bounds" {
    const source = [_]u8{ 7, 8, 7, 9 };
    const bytes: c.bytes_t = .{ .items = &source, .len = source.len };

    try std.testing.expectEqual(@as(usize, 0), c.bytes_find(bytes, 7));
    try std.testing.expectEqual(@as(usize, 2), c.bytes_rfind(bytes, 7));
    try std.testing.expectEqual(@as(usize, c.SIGMA_NPOS), c.bytes_find(bytes, 6));
    try std.testing.expectEqual(@as(usize, c.SIGMA_NPOS), c.bytes_rfind(.{ .items = null, .len = 0 }, 0));
}

test "modern memory primitives stay within bounds" {
    const left = [_]u8{ 1, 2, 3, 4 };
    const right = [_]u8{ 1, 2, 4, 0 };

    try std.testing.expectEqual(@as(usize, 2), c.mem_find(&left, 3, 3));
    try std.testing.expectEqual(@as(usize, c.SIGMA_NPOS), c.mem_find(&left, 2, 3));
    try std.testing.expectEqual(@as(usize, c.SIGMA_NPOS), c.mem_find(null, 0, 0));
    try std.testing.expect(c.mem_cmp(&left, &right, 2) == 0);
    try std.testing.expect(c.mem_cmp(&left, &right, 3) < 0);
    try std.testing.expect(c.mem_cmp(null, null, 0) == 0);
}

test "SIMD find and compare handle vector boundaries and tails" {
    var left = [_]u8{0x5a} ** 257;
    var right: [257]u8 = undefined;
    right = left;

    const boundaries = [_]usize{ 0, 15, 16, 31, 32, 63, 64, 127, 128, 255, 256 };
    for (boundaries) |index| {
        const original = right[index];
        right[index] +%= 1;
        try std.testing.expect(c.mem_cmp(&left, &right, left.len) < 0);
        right[index] = original;
    }

    left[256] = 0xff;
    try std.testing.expectEqual(@as(usize, 256), c.mem_find(&left, left.len, 0xff));
    try std.testing.expectEqual(@as(usize, c.SIGMA_NPOS), c.mem_find(&left, 256, 0xff));
}

test "sigma_str_hash" {
    const left = c.sigma_str_hash(c.str_from_cstr("left"));
    const right = c.sigma_str_hash(c.str_from_cstr("right"));
    try std.testing.expect(left != right);
}

test "sigma_str_map_init and deinit" {
    var map: c.sigma_str_map = undefined;
    try std.testing.expect(c.sigma_str_map_init(&map, testAllocator(), 4));
    try std.testing.expectEqual(@as(usize, 4), map.cap);
    c.sigma_str_map_deinit(&map);
    try std.testing.expectEqual(@as(usize, 0), map.cap);
}

test "sigma_str_map_put" {
    var map: c.sigma_str_map = undefined;
    try std.testing.expect(c.sigma_str_map_init(&map, testAllocator(), 0));
    defer c.sigma_str_map_deinit(&map);
    try std.testing.expect(c.sigma_str_map_put(&map, c.str_from_cstr("key"), c.str_from_cstr("value")));
}

test "sigma_str_map_get" {
    var map: c.sigma_str_map = undefined;
    try std.testing.expect(c.sigma_str_map_init(&map, testAllocator(), 0));
    defer c.sigma_str_map_deinit(&map);
    const key = c.str_from_cstr("key");
    try std.testing.expect(c.sigma_str_map_put(&map, key, c.str_from_cstr("value")));
    try std.testing.expect(c.sigma_str_map_get(&map, key) != null);
}

test "sigma_str_map_has" {
    var map: c.sigma_str_map = undefined;
    try std.testing.expect(c.sigma_str_map_init(&map, testAllocator(), 0));
    defer c.sigma_str_map_deinit(&map);
    const key = c.str_from_cstr("key");
    try std.testing.expect(c.sigma_str_map_put(&map, key, c.str_from_cstr("value")));
    try std.testing.expect(c.sigma_str_map_has(&map, key));
}

test "sigma_line_reader_init and deinit" {
    var reader: c.sigma_line_reader = undefined;
    c.sigma_line_reader_init(&reader, -1, testAllocator());
    try std.testing.expectEqual(@as(c_int, -1), reader.fd);
    c.sigma_line_reader_deinit(&reader);
    try std.testing.expect(reader.reached_eof);
}

test "sigma_line_reader_next" {
    var fds: [2]c_int = undefined;
    try std.testing.expectEqual(@as(c_int, 0), c.pipe(&fds));
    _ = c.write(fds[1], "line\n", 5);
    _ = c.close(fds[1]);
    defer _ = c.close(fds[0]);
    var reader: c.sigma_line_reader = undefined;
    c.sigma_line_reader_init(&reader, fds[0], testAllocator());
    defer c.sigma_line_reader_deinit(&reader);
    const line = c.sigma_line_reader_next(&reader);
    try std.testing.expectEqual(@as(c_uint, c.sigma_line_ok), line.tag);
    try std.testing.expectEqualSlices(u8, "line", line.line.items[0..line.line.len]);
}

test "line reader scans long lines and clears its arena" {
    allocation_count = 0;
    free_count = 0;
    var input = [_]u8{'x'} ** 9001;
    input[input.len - 1] = '\n';
    var fds: [2]c_int = undefined;
    try std.testing.expectEqual(@as(c_int, 0), c.pipe(&fds));
    try std.testing.expectEqual(@as(isize, input.len), c.write(fds[1], &input, input.len));
    _ = c.close(fds[1]);
    defer _ = c.close(fds[0]);

    var reader: c.sigma_line_reader = undefined;
    c.sigma_line_reader_init(&reader, fds[0], testAllocator());
    const line = c.sigma_line_reader_next(&reader);
    try std.testing.expectEqual(@as(c_uint, c.sigma_line_ok), line.tag);
    try std.testing.expectEqual(@as(usize, input.len - 1), line.line.len);
    try std.testing.expect(reader.arena.blocks != null);
    c.sigma_line_reader_deinit(&reader);
    try std.testing.expect(reader.arena.blocks == null);
    try std.testing.expect(allocation_count > 0);
    try std.testing.expectEqual(allocation_count, free_count);
}

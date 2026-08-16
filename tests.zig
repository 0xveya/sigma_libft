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

fn cString(ptr: [*c]u8) []u8 {
    return std.mem.span(@as([*:0]u8, @ptrCast(ptr)));
}

fn noopDelete(_: ?*anyopaque) callconv(.c) void {}

fn incrementByte(_: c_uint, byte: [*c]u8) callconv(.c) void {
    byte.* += 1;
}

fn incrementMapped(_: c_uint, byte: u8) callconv(.c) u8 {
    return byte + 1;
}

var iter_count: usize = 0;
fn countNode(_: ?*anyopaque) callconv(.c) void {
    iter_count += 1;
}

fn identityContent(content: ?*anyopaque) callconv(.c) ?*anyopaque {
    return content;
}

fn pipeText(comptime writeFn: fn (c_int) void) ![]u8 {
    var fds: [2]c_int = undefined;
    try std.testing.expectEqual(@as(c_int, 0), c.pipe(&fds));
    writeFn(fds[1]);
    _ = c.close(fds[1]);
    var buffer: [128]u8 = undefined;
    const count = c.read(fds[0], &buffer, buffer.len);
    _ = c.close(fds[0]);
    try std.testing.expect(count >= 0);
    return std.testing.allocator.dupe(u8, buffer[0..@intCast(count)]);
}

fn writeChar(fd: c_int) void {
    c.ft_putchar_fd('x', fd);
}
fn writeString(fd: c_int) void {
    c.ft_putstr_fd(@constCast("hello"), fd);
}
fn writeLine(fd: c_int) void {
    c.ft_putendl_fd(@constCast("hello"), fd);
}
fn writeNumber(fd: c_int) void {
    c.ft_putnbr_fd(-2147483648, fd);
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

test "ft_isalpha" {
    try std.testing.expect(c.ft_isalpha('A') != 0 and c.ft_isalpha('1') == 0);
}

test "ft_isdigit" {
    try std.testing.expect(c.ft_isdigit('7') != 0 and c.ft_isdigit('x') == 0);
}

test "ft_isalnum" {
    try std.testing.expect(c.ft_isalnum('7') != 0 and c.ft_isalnum('-') == 0);
}

test "ft_isascii" {
    try std.testing.expect(c.ft_isascii(127) != 0 and c.ft_isascii(128) == 0);
}

test "ft_isprint" {
    try std.testing.expect(c.ft_isprint(' ') != 0 and c.ft_isprint('\n') == 0);
}

test "ft_tolower" {
    try std.testing.expectEqual(@as(c_int, 'a'), c.ft_tolower('A'));
}

test "ft_toupper" {
    try std.testing.expectEqual(@as(c_int, 'A'), c.ft_toupper('a'));
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

test "ft_split" {
    const parts = c.ft_split("::one:two::", ':');
    try std.testing.expect(parts != null);
    defer {
        var index: usize = 0;
        while (parts[index] != null) : (index += 1) std.c.free(parts[index]);
        std.c.free(@ptrCast(parts));
    }
    try std.testing.expectEqualStrings("one", cString(parts[0]));
    try std.testing.expectEqualStrings("two", cString(parts[1]));
    try std.testing.expect(parts[2] == null);
}

test "ft_strchr" {
    const found = c.ft_strchr("abc", 'b');
    try std.testing.expect(found != null);
    try std.testing.expectEqualStrings("bc", cString(found));
}

test "ft_strdup" {
    const duplicate = c.ft_strdup("hello");
    try std.testing.expect(duplicate != null);
    defer std.c.free(duplicate);
    try std.testing.expectEqualStrings("hello", cString(duplicate));
}

test "ft_striteri" {
    var text = [_:0]u8{ 'a', 'b', 'c' };
    c.ft_striteri(&text, incrementByte);
    try std.testing.expectEqualSlices(u8, "bcd", text[0..3]);
}

test "ft_strjoin" {
    const joined = c.ft_strjoin("sigma_", "libft");
    try std.testing.expect(joined != null);
    defer std.c.free(joined);
    try std.testing.expectEqualStrings("sigma_libft", cString(joined));
}

test "ft_strlcat" {
    var dst = [_:0]u8{ 'a', 'b', 0, 0, 0, 0, 0, 0 };
    try std.testing.expectEqual(@as(usize, 4), c.ft_strlcat(&dst, "cd", dst.len));
    try std.testing.expectEqualStrings("abcd", cString(&dst));
}

test "ft_strlcpy" {
    var dst = [_]u8{0} ** 4;
    try std.testing.expectEqual(@as(usize, 5), c.ft_strlcpy(&dst, "hello", dst.len));
    try std.testing.expectEqualSlices(u8, "hel\x00", &dst);
}

test "ft_strlen" {
    try std.testing.expectEqual(@as(usize, 128), c.ft_strlen("x" ** 128));
}

test "ft_strmapi" {
    const mapped = c.ft_strmapi("abc", incrementMapped);
    try std.testing.expect(mapped != null);
    defer std.c.free(mapped);
    try std.testing.expectEqualStrings("bcd", cString(mapped));
}

test "ft_strncmp" {
    try std.testing.expect(c.ft_strncmp("abc", "abd", 3) < 0);
}

test "ft_strnstr" {
    const found = c.ft_strnstr("hello world", "world", 11);
    try std.testing.expect(found != null);
    try std.testing.expectEqualStrings("world", cString(found));
}

test "ft_strrchr" {
    const found = c.ft_strrchr("abca", 'a');
    try std.testing.expect(found != null);
    try std.testing.expectEqualStrings("a", cString(found));
}

test "ft_strtrim" {
    const trimmed = c.ft_strtrim("  hello  ", " ");
    try std.testing.expect(trimmed != null);
    defer std.c.free(trimmed);
    try std.testing.expectEqualStrings("hello", cString(trimmed));
}

test "ft_substr" {
    const substring = c.ft_substr("hello", 1, 3);
    try std.testing.expect(substring != null);
    defer std.c.free(substring);
    try std.testing.expectEqualStrings("ell", cString(substring));
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

test "ft_putchar_fd" {
    const output = try pipeText(writeChar);
    defer std.testing.allocator.free(output);
    try std.testing.expectEqualStrings("x", output);
}

test "ft_putstr_fd" {
    const output = try pipeText(writeString);
    defer std.testing.allocator.free(output);
    try std.testing.expectEqualStrings("hello", output);
}

test "ft_putendl_fd" {
    const output = try pipeText(writeLine);
    defer std.testing.allocator.free(output);
    try std.testing.expectEqualStrings("hello\n", output);
}

test "ft_putnbr_fd" {
    const output = try pipeText(writeNumber);
    defer std.testing.allocator.free(output);
    try std.testing.expectEqualStrings("-2147483648", output);
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

test "sigma_str_from_cstr" {
    const string = c.sigma_str_from_cstr("hello");
    try std.testing.expectEqual(@as(usize, 5), string.len);
}

test "sigma_str_eq" {
    try std.testing.expect(c.sigma_str_eq(c.sigma_str_from_cstr("same"), c.sigma_str_from_cstr("same")));
    try std.testing.expect(!c.sigma_str_eq(c.sigma_str_from_cstr("same"), c.sigma_str_from_cstr("other")));
}

test "sigma_str_hash" {
    const left = c.sigma_str_hash(c.sigma_str_from_cstr("left"));
    const right = c.sigma_str_hash(c.sigma_str_from_cstr("right"));
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
    try std.testing.expect(c.sigma_str_map_put(&map, c.sigma_str_from_cstr("key"), c.sigma_str_from_cstr("value")));
}

test "sigma_str_map_get" {
    var map: c.sigma_str_map = undefined;
    try std.testing.expect(c.sigma_str_map_init(&map, testAllocator(), 0));
    defer c.sigma_str_map_deinit(&map);
    const key = c.sigma_str_from_cstr("key");
    try std.testing.expect(c.sigma_str_map_put(&map, key, c.sigma_str_from_cstr("value")));
    try std.testing.expect(c.sigma_str_map_get(&map, key) != null);
}

test "sigma_str_map_has" {
    var map: c.sigma_str_map = undefined;
    try std.testing.expect(c.sigma_str_map_init(&map, testAllocator(), 0));
    defer c.sigma_str_map_deinit(&map);
    const key = c.sigma_str_from_cstr("key");
    try std.testing.expect(c.sigma_str_map_put(&map, key, c.sigma_str_from_cstr("value")));
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

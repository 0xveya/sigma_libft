const std = @import("std");

const c_flags = [_][]const u8{
    "-std=c23",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-pedantic-errors",
    "-Wshadow",
    "-Wconversion",
    "-Wdouble-promotion",
    "-Wformat=2",
    "-Wundef",
};

const Simd = enum { auto, scalar, sse2, avx2 };

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const sigma_malloc = b.dependency("sigma_malloc", .{});
    const simd = b.option(Simd, "simd", "Select SIMD dispatch: auto, scalar, sse2, or avx2") orelse .auto;

    const lib = b.addLibrary(.{
        .name = "sigma_libft",
        .linkage = .static,
        .root_module = b.createModule(.{ .target = target, .optimize = optimize }),
    });
    configureC(b, lib.root_module, sigma_malloc, simd);
    b.installArtifact(lib);
    lib.installHeadersDirectory(b.path("include"), "", .{});

    const translated = b.addTranslateC(.{
        .root_source_file = b.path("include/sigma/libft.h"),
        .target = target,
        .optimize = optimize,
    });
    translated.addIncludePath(b.path("include"));
    translated.addIncludePath(sigma_malloc.path("include"));
    _ = b.addModule("sigma_libft", .{
        .root_source_file = b.path("src/sigma_libft.zig"),
        .target = target,
        .optimize = optimize,
        .imports = &.{.{ .name = "c", .module = translated.createModule() }},
    });

    const tests = b.addTest(.{
        .name = "sigma-libft-tests",
        .root_module = b.createModule(.{
            .root_source_file = b.path("tests.zig"),
            .target = target,
            .optimize = optimize,
            .imports = &.{.{ .name = "c", .module = translated.createModule() }},
        }),
        .test_runner = .{
            .path = b.path("test_runner.zig"),
            .mode = .simple,
        },
    });
    tests.use_llvm = true;
    configureC(b, tests.root_module, sigma_malloc, simd);
    tests.root_module.addCSourceFile(.{
        .file = b.path("tests/vec_fixture.c"),
        .flags = &c_flags,
    });
    tests.root_module.addCSourceFile(.{
        .file = b.path("tests/string_fixture.c"),
        .flags = &c_flags,
    });
    b.step("test", "Run sigma_libft tests").dependOn(&b.addRunArtifact(tests).step);
}

fn configureC(b: *std.Build, module: *std.Build.Module, sigma_malloc: *std.Build.Dependency, simd: Simd) void {
    module.link_libc = true;
    module.addIncludePath(b.path("include"));
    module.addIncludePath(sigma_malloc.path("include"));
    switch (simd) {
        .auto => {},
        .scalar => module.addCMacro("SIGMA_SIMD_FORCE_SCALAR", "1"),
        .sse2 => module.addCMacro("SIGMA_SIMD_FORCE_SSE2", "1"),
        .avx2 => module.addCMacro("SIGMA_SIMD_FORCE_AVX2", "1"),
    }
    module.addCSourceFiles(.{
        .root = b.path("src"),
        .files = &source_files,
        .flags = &c_flags,
    });
}

const source_files = [_][]const u8{
    "ascii/classify.c",
    "bytes/basic.c",
    "bytes/compare.c",
    "bytes/find.c",
    "collections/hash_map.c",
    "conversion/ft_itoa.c",
    "conversion/parse_i32.c",
    "io/printf/conversion/integer_digits.c",
    "io/printf/conversion/print_char.c",
    "io/printf/conversion/print_hex.c",
    "io/printf/conversion/print_int.c",
    "io/printf/conversion/print_percent.c",
    "io/printf/conversion/print_pointer.c",
    "io/printf/conversion/print_string.c",
    "io/printf/conversion/print_uint.c",
    "io/printf/core/printf.c",
    "io/printf/core/printf_dispatch.c",
    "io/printf/format/parse.c",
    "io/printf/format/parse_utils.c",
    "io/printf/support/writer.c",
    "list/ft_lstadd_back.c",
    "list/ft_lstadd_front.c",
    "list/ft_lstclear.c",
    "list/ft_lstdelone.c",
    "list/ft_lstiter.c",
    "list/ft_lstlast.c",
    "list/ft_lstmap.c",
    "list/ft_lstnew.c",
    "list/ft_lstsize.c",
    "memory/ft_bzero.c",
    "memory/ft_calloc.c",
    "memory/ft_memchr.c",
    "memory/ft_memcmp.c",
    "memory/ft_memcpy.c",
    "memory/ft_memmove.c",
    "memory/ft_memset.c",
    "memory/cmp.c",
    "memory/copy.c",
    "memory/find.c",
    "reader/reader.c",
    "reader/scan.c",
    "str/basic.c",
    "str/compare.c",
    "str/find.c",
    "str/split.c",
    "str/trim.c",
    "string/string.c",
    "unicode/unicode.c",
};

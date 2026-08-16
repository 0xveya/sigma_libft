const std = @import("std");

const c_flags = [_][]const u8{
    "-std=c23",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wdouble-promotion",
    "-Wformat=2",
    "-Wundef",
};

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const sigma_malloc = b.dependency("sigma_malloc", .{});

    const lib = b.addLibrary(.{
        .name = "sigma_libft",
        .linkage = .static,
        .root_module = b.createModule(.{ .target = target, .optimize = optimize }),
    });
    configureC(b, lib.root_module, sigma_malloc);
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
    });
    configureC(b, tests.root_module, sigma_malloc);
    b.step("test", "Run sigma_libft tests").dependOn(&b.addRunArtifact(tests).step);
}

fn configureC(b: *std.Build, module: *std.Build.Module, sigma_malloc: *std.Build.Dependency) void {
    module.link_libc = true;
    module.addIncludePath(b.path("include"));
    module.addIncludePath(sigma_malloc.path("include"));
    module.addCSourceFiles(.{
        .root = b.path("src"),
        .files = &source_files,
        .flags = &c_flags,
    });
}

const source_files = [_][]const u8{};

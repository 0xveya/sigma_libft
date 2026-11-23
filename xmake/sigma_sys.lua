local sigma_sys_root = path.absolute(path.join(os.scriptdir(), ".."))

local runtime_only = has_config("sigma_runtime_only")

target(runtime_only and "sigma_libft_runtime" or "sigma_sys")
    set_kind("static")
    set_languages("c23")
    set_warnings("all", "extra", "pedantic")
    add_cflags("-ffreestanding", "-fno-builtin", "-fno-stack-protector", "-Wshadow", "-Wconversion", {force = true})
    add_includedirs(path.join(sigma_sys_root, "include"), {public = true})
    add_headerfiles(path.join(sigma_sys_root, "include/sigma/result.h"), {prefixdir = "sigma"})
    add_headerfiles(path.join(sigma_sys_root, "include/sigma/meta.h"), {prefixdir = "sigma"})
    add_headerfiles(path.join(sigma_sys_root, "include/sigma/sys.h"), {prefixdir = "sigma"})
    add_headerfiles(path.join(sigma_sys_root, "include/sigma/types.h"), {prefixdir = "sigma"})
    add_headerfiles(path.join(sigma_sys_root, "include/sigma/sys/(**.h)"), {prefixdir = "sigma/sys"})
    add_files(path.join(sigma_sys_root, "src/sys/x86_64_linux.c"))
    if runtime_only then
        set_toolchains("clang")
        add_defines("SIGMA_RUNTIME_ONLY", {public = true})
        for _, header in ipairs({"attributes", "bytes", "diagnostic", "mem",
                                 "printf", "qol", "rune", "str",
                                 "type_registry", "writer"}) do
            add_headerfiles(path.join(sigma_sys_root, "include/sigma/" .. header .. ".h"), {prefixdir = "sigma"})
        end
        for _, source in ipairs({"cmp", "copy", "find", "ft_bzero", "ft_memcpy", "ft_memset"}) do
            add_files(path.join(sigma_sys_root, "src/memory/" .. source .. ".c"))
        end
        for _, source in ipairs({"str/basic.c", "str/compare.c", "str/find.c",
                                 "bytes/compare.c", "bytes/find.c",
                                 "io/printf/format/*.c", "io/printf/support/writer.c"}) do
            add_files(path.join(sigma_sys_root, "src", source))
        end
    end
    on_load(function (target)
        assert(target:is_plat("linux") and target:is_arch("x86_64", "x64"),
               "sigma_sys currently supports Linux x86_64 only")
    end)

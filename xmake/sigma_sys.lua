local sigma_sys_root = path.absolute(path.join(os.scriptdir(), ".."))

target("sigma_sys")
    set_kind("static")
    set_languages("c23")
    set_warnings("all", "extra", "pedantic")
    add_cflags("-ffreestanding", "-fno-builtin", "-Wshadow", "-Wconversion", {force = true})
    add_includedirs(path.join(sigma_sys_root, "include"), {public = true})
    add_headerfiles(path.join(sigma_sys_root, "include/sigma/sys.h"), {prefixdir = "sigma"})
    add_files(path.join(sigma_sys_root, "src/sys/x86_64_linux.c"))
    on_load(function (target)
        assert(target:is_plat("linux") and target:is_arch("x86_64", "x64"),
               "sigma_sys currently supports Linux x86_64 only")
    end)

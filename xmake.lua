set_project("sigma_libft")
set_version("0.4.1")
set_languages("c23")
add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})
includes("xmake/sigma_sys.lua")

option("sigma_sys_only")
    set_default(false)
    set_showmenu(true)
option_end()

if not has_config("sigma_sys_only") then

package("sigma_malloc")
    set_homepage("https://github.com/0xveya/sigma-malloc")
    set_description("Sigma's composable C23 allocator")
    add_urls("https://github.com/0xveya/sigma-malloc.git")
    on_install(function (package)
        import("package.tools.xmake").install(package)
    end)
package_end()
add_requires("sigma_malloc v0.2.0", {system = false})

option("simd")
    set_default("native")
    set_values("native", "scalar", "sse2", "avx2")
option_end()

option("sigma_diagnostics")
    set_default(false)
    set_showmenu(true)
    set_description("Render Sigma compiler errors through sigma-diagnostics")
option_end()

local sigma_projectdir = os.scriptdir()

toolchain("sigma-clang")
    set_kind("standalone")
    on_load(function (toolchain)
        local compiler = path.join(sigma_projectdir, "build/tools/sigma-diagnostics/clang")
        os.mkdir(path.directory(compiler))
        os.vrunv("go", {"build", "-C", path.join(sigma_projectdir, "tools/sigma-diagnostics"), "-o", compiler, "."})
        toolchain:set("toolset", "cc", compiler)
        toolchain:set("toolset", "ld", "clang")
        toolchain:set("toolset", "ar", "llvm-ar")
    end)
toolchain_end()

if has_config("sigma_diagnostics") then
    set_toolchains("sigma-clang")
else
    set_toolchains("clang")
end
set_toolset("ld", "clang")

local warnings = {"all", "extra", "pedantic"}
target("sigma_libft")
    set_kind("static")
    set_warnings(table.unpack(warnings))
    add_cflags("-ffreestanding", "-fno-builtin", "-fno-stack-protector", "-Wshadow", "-Wconversion", "-Wdouble-promotion", "-Wformat=2", "-Wundef", {force = true})
    add_includedirs("include", {public = true})
    add_packages("sigma_malloc", {public = true})
    add_deps("sigma_sys", {public = true})
    add_headerfiles("include/(sigma/*.h)")
    add_files("src/**.c", {excludes = "src/sys/**.c"})
    local simd = get_config("simd") or "native"
    if simd ~= "native" then
        add_defines("SIGMA_SIMD_FORCE_" .. simd:upper())
    end

target("sigma-libft-tests")
    set_kind("binary")
    set_default(false)
    set_warnings(table.unpack(warnings))
    add_cflags("-Wshadow", "-Wconversion", "-Wdouble-promotion", "-Wformat=2", "-Wundef", {force = true})
    add_deps("sigma_libft")
    add_files("tests/test_main.c", "tests/core.c", "tests/vec_fixture.c", "tests/string_fixture.c", "tests/format_fixture.c", "tests/compile_contracts.c")
    local tests = {
        "vector codegen preserves ownership contracts",
        "owned strings and string vectors preserve ownership contracts",
        "format arguments normalize values and borrow views",
        "runtime formatting writes default integers and borrowed strings",
        "Sigma format fields parse sequential presentation options",
        "preprocessor argument counting and mapping cover zero through 64",
        "format convenience macros map values and accept no arguments",
        "custom formatter vtables borrow and dispatch values",
        "sigma_print formats literals to stdout",
        "printf wrappers target buffers, fds, and owning strings",
        "parse i32 distinguishes valid, invalid, and overflow input",
        "memory primitives handle exact byte ranges",
        "SIMD memory paths preserve guards and tails",
        "string map grows and retrieves values",
        "line reader preserves buffered lines and reports eof",
        "ASCII table classifies and converts every byte",
        "Unicode database classifies and maps representative runes",
        "ft_atoi", "ft_itoa", "ft_bzero", "ft_calloc", "ft_memchr", "ft_memcmp",
        "ft_memcpy", "ft_memmove", "ft_memset", "ft_lstnew", "ft_lstadd_front",
        "ft_lstadd_back", "ft_lstlast", "ft_lstsize", "ft_lstiter", "ft_lstdelone",
        "ft_lstclear", "ft_lstmap", "str converts from and to C strings",
        "str slices and compares bounded contents", "str finds bytes and trims ASCII whitespace",
        "str scalar split preserves empty fields", "bytes slices immutable and mutable views",
        "bytes compares bounded contents", "bytes finds values within bounds",
        "modern memory primitives stay within bounds",
        "SIMD find and compare handle vector boundaries and tails", "sigma_str_hash",
        "sigma_str_map_init and deinit", "sigma_str_map_put", "sigma_str_map_get",
        "sigma_str_map_has", "sigma_line_reader_init and deinit",
        "sigma_line_reader_next", "line reader scans long lines and clears its arena"
    }
    for _, name in ipairs(tests) do
        add_tests(name, {runargs = {name}})
    end

target("sigma-sys-tests")
    set_kind("binary")
    set_default(false)
    set_warnings(table.unpack(warnings))
    add_deps("sigma_sys")
    add_files("tests/sys.c")
    add_tests("tagged write errors and mmap lifecycle")

target("sigma-libft-diagnostics")
    set_kind("object")
    set_default(false)
    add_includedirs("include")
    add_packages("sigma_malloc")
    add_files("tests/compile_fail/diagnostics.c")

task("diagnostics")
    on_run(function ()
        import("core.project.project")
        local package = assert(project.target("sigma-libft-diagnostics"):pkg("sigma_malloc"))
        os.execv("go", {"run", "./tools/sigma-diagnostics/main.go", "--expect-failure", "--", "clang", "-std=c23", "-Iinclude", "-I" .. path.join(package:installdir(), "include"), "-c", "tests/compile_fail/diagnostics.c", "-o", os.nuldev()})
    end)
    set_menu({usage = "xmake diagnostics", description = "Render intentional compile-time diagnostics"})
task_end()
end

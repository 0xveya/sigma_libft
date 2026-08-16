.PHONY: compiledb compiledb-commands clean

MAKEFLAGS += -j

C_SOURCES := $(shell find src -type f -name '*.c' | sort)
COMPILEDB_TARGETS := $(C_SOURCES:%=compiledb-%)
COMPILEDB_FLAGS := -std=c23 -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
	-Wdouble-promotion -Wformat=2 -Wundef -Iinclude -I../sigma_malloc/include

.PHONY: $(COMPILEDB_TARGETS)

compiledb:
	compiledb --overwrite make compiledb-commands

compiledb-commands: $(COMPILEDB_TARGETS)

$(COMPILEDB_TARGETS):
	clang $(COMPILEDB_FLAGS) -fsyntax-only $(patsubst compiledb-%,%,$@)

clean:
	rm -rf zig-out .zig-cache compile_commands.json .bear-fingerprints

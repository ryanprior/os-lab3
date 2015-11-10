CC=g++
DEBUG_FLAGS=-ggdb3
WARN_FLAGS=-Wall -Wno-unused-local-typedefs
CFLAGS=$(WARN_FLAGS) $(DEBUG_FLAGS)
TEST_BIN=bin/test

.PHONY: all build

all: $(TEST_BIN)
build/%.o: src/%.cpp src/%.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<
build/%.o: src/%.cpp build
	@mkdir -p build
	$(CC) $(CLFLAGS) -c -o $@ $<


# dummy test binary
$(TEST_BIN): build/main.o build/Process.o
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^


# development tools
compile_commands.json: Makefile
	bear -o $@ -- make -Bk
clean:
	rm -rf build $(TEST_BIN)

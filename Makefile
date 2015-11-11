CC=g++
DEBUG_FLAGS=-ggdb3
WARN_FLAGS=-Wall -Wno-unused-local-typedefs
CFLAGS=$(WARN_FLAGS) $(DEBUG_FLAGS)
TEST_BIN=bin/test
SIGNALS=lib/Signals/Signal.h

.PHONY: all

all: $(TEST_BIN)
build/%.o: src/%.cpp src/%.h $(SIGNALS)
	@mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<
build/%.o: src/%.cpp $(SIGNALS)
	@mkdir -p build
	$(CC) $(CLFLAGS) -c -o $@ $<


# libs
$(SIGNALS):
	git submodule update --init lib/Signals


# dummy test binary
$(TEST_BIN): build/main.o build/Process.o build/Simulator.o
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^


# development tools
compile_commands.json: Makefile
	bear -o $@ -- make -Bk
clean:
	rm -rf build $(TEST_BIN)

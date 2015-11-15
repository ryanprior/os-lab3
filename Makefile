SIG_LIB=include/Signals/Signal.h

.PHONY: all compile
all: $(SIG_LIB) compile

compile: build/Makefile
	@$(MAKE) -C build

include/Signals/Signal.h:
	git submodule update --init include/Signals

build/Makefile: configure
	@mkdir -p build
	cd build && ../configure

configure: configure.ac Makefile.am
	autoreconf -is

compile_commands.json: build/Makefile
	bear -o ../compile_commands.json -- $(MAKE) -C build -Bk

clean:
	$(MAKE) -C build clean

clean-config:
	find . -maxdepth 1 -type l -delete
	rm -Rf build autom4te.cache
	rm -f aclocal.m4 config.h.in configure Makefile.in

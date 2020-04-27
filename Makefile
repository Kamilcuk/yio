# Makefile

define NL


endef

.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-print-directories
unexport MAKEFLAGS

CMAKE_BUILD_TYPE ?= Debug
# CMAKE_BUILD_TYPE ?= Release
 
B_SUFFIX ?=
B ?= _build/$(CMAKE_BUILD_TYPE)$(B_SUFFIX)

NICE += $(shell hash nice 2>/dev/null >/dev/null && echo nice)
NICE += $(shell hash ionice 2>/dev/null >/dev/null && echo ionice)

CTEST := $(NICE) ctest
CTESTFLAGS += --output-on-failure 
CTESTFLAGS += -j $(shell nproc) # --verbose --rerun-failed

CMAKE_C_FLAGS ?= 

CMAKE := $(NICE) cmake
CMAKEFLAGS += -S .
CMAKEFLAGS_GENERATOR ?= $(shell hash ninja 2>/dev/null >/dev/null && echo -GNinja)
CMAKEFLAGS += $(CMAKEFLAGS_GENERATOR)
CMAKEFLAGS += -D CMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
CMAKEFLAGS += -D CMAKE_RUNTIME_OUTPUT_DIRECTORY=bin
CMAKEFLAGS += -D CMAKE_LIBRARY_OUTPUT_DIRECTORY=lib
CMAKEFLAGS += -D CMAKE_EXECUTABLE_SUFFIX=.out
CMAKEFLAGS += -D CMAKE_VERBOSE_MAKEFILE=ON
CMAKEFLAGS += -D CMAKE_C_FLAGS="$(CMAKE_C_FLAGS)"
CMAKEFLAGS += -D BUILD_TESTING=ON

SHELL = bash

all: build # test

configure:
	$(CMAKE) -P cmake/_test.cmake
	$(CMAKE) -B $(B) $(CMAKEFLAGS)
build: configure
	@echo $(CMAKE) --build $(B)
	@stdbuf -oL $(CMAKE) --build $(B) 2> >( \
		sed -u 's@^[^ ]*/gen/@src/@' >&2) | \
		sed -u 's@^[^ ]*/gen/@src/@'
test: build
	cd $(B) && $(CTEST) $(CTESTFLAGS)

cmake-gui:
	cmake-gui -B $(B) $(CMAKEFLAGS)
	
ccmake:
	ccmake -B $(B) $(CMAKEFLAGS)

gitlab-ci:
	+$(MAKE) -k CMAKE_BUILD_TYPE=Release memcheck sanitize coverage cdash

memcheck: build
	cd $(B) && $(CTEST) -T memcheck $(CTESTFLAGS)

sanitize:
	+$(MAKE) B_SUFFIX=_sanitize CMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer" test

doxygen:
	doxygen ./doc/Doxyfile
	mv ./public/html/* ./public/

coverage:
	+$(MAKE) B_SUFFIX=_coverage CMAKE_C_FLAGS="--coverage -g" .coverage

.coverage: test
	gcovr -r . -e test -e _build $(B)

cdash: D=_build/cdash
cdash:
	mkdir -p "$(D)"
	find . -maxdepth 1 -mindepth 1 '!' -name _build | xargs -t -d'\n' cp -at $(D)
	+$(MAKE) -C "$(D)" B=$(D) CMAKE_BUILD_TYPE=Release CMAKEFLAGS_GENERATOR="" CMAKE_C_FLAGS="--coverage -O" .cdash
.cdash: configure
	cd "$(B)" && pwd && ctest -T all

clean:
	if [ -e _build ]; then rm -r _build; fi

distclean: clean
	if [ -e public ]; then rm -r public; fi

.PHONY: all $(MAKECMDGOALS)



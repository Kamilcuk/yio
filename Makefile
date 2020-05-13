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

SED_FIX_PATHS = sed -u 's@^[^ ]*/gen/@src/@; s@^\.\./\.\./test@test@'
GEN_TO_SRC = 2> >($(SED_FIX_PATHS) >&2) > >($(SED_FIX_PATHS))
STDBUF = $(shell hash stdbuf 2>/dev/null >/dev/null && echo stdbuf -oL -eL) 

all: build test

configure:
	$(STDBUF) $(CMAKE) -B $(B) $(CMAKEFLAGS)
build_gen: configure
	ln -vfs $(B)/src/gen gen ||:
	$(STDBUF) $(CMAKE) --build $(B) --target _yio_gen $(GEN_TO_SRC)
build_yio: configure
	$(STDBUF) $(CMAKE) --build $(B) --target yio $(GEN_TO_SRC)
build: configure
	$(STDBUF) $(CMAKE) --build $(B) --target all $(GEN_TO_SRC)
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

_build/Doxyfile: doc/Doxyfile build_gen
	sed \
		-e "/STRIP_FROM_PATH/s@<ABSOLUTE_GEN_SEE_MAKEFILE>@$$(readlink -f ./gen)@" \
		doc/Doxyfile > _build/Doxyfile
public/html:
	mkdir -p public
	ln -s . ./public/html
.PHONY: doxygen
doxygen: build_gen public/html _build/Doxyfile
	doxygen _build/Doxyfile

.PHONY: coverage
coverage:
	+$(MAKE) B_SUFFIX=_coverage CMAKE_C_FLAGS="--coverage -g" .coverage

.coverage: test
	gcovr -r . -e test -e _build $(B)

cdash: export ROOT = $(PWD)/_build/cdash
cdash: export D = ${ROOT}/source
cdash: export B = ${ROOT}/_build
cdash:
	rm -rf $(ROOT)
	mkdir -p $(D)
	git ls-tree -r --name-only @ | xargs -t -d'\n' cp --parents -a -t $(D)
	+$(MAKE) -C "$(D)" B=$(B) CMAKE_BUILD_TYPE=Release CMAKE_C_FLAGS="--coverage -O" .cdash
.cdash: configure
	# cmake --build $(B)
	cd "$(B)" && pwd && ctest -T all

clean:
	if [ -e _build ]; then rm -r _build; fi

distclean: clean
	if [ -e public ]; then rm -r public; fi

install: build
	$(CMAKE) --build $(B) --target install
	
uninstall: build
	$(CMAKE) --build $(B) --target yio_uninstall

.PHONY: all $(MAKECMDGOALS)



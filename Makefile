# Makefile

.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-print-directories

CMAKE_BUILD_TYPE ?= Debug
# CMAKE_BUILD_TYPE ?= Release
 
B ?= _build/$(CMAKE_BUILD_TYPE)

NICE += $(shell hash nice 2>/dev/null >/dev/null && echo nice)
NICE += $(shell hash ionice 2>/dev/null >/dev/null && echo ionice)

CTEST := $(NICE) ctest
CTESTFLAGS += --output-on-failure 
CTESTFLAGS += -j $(shell nproc) # --verbose --rerun-failed

CMAKE_C_FLAGS ?= 

CMAKE := $(NICE) cmake
CMAKEFLAGS += -S .
CMAKEFLAGS += $(shell hash ninja 2>/dev/null >/dev/null && echo -GNinja)
CMAKEFLAGS += -D CMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
CMAKEFLAGS += -D CMAKE_RUNTIME_OUTPUT_DIRECTORY=bin
CMAKEFLAGS += -D CMAKE_LIBRARY_OUTPUT_DIRECTORY=lib
CMAKEFLAGS += -D CMAKE_EXECUTABLE_SUFFIX=.out
CMAKEFLAGS += -D CMAKE_VERBOSE_MAKEFILE=ON
CMAKEFLAGS += -D CMAKE_C_FLAGS="$(CMAKE_C_FLAGS)"
CMAKEFLAGS += -D BUILD_TESTING=ON

all: test

build:
	$(CMAKE) -B $(B) $(CMAKEFLAGS)
	$(CMAKE) --build $(B)
	
test: build
	cd $(B) && $(CTEST) $(CTESTFLAGS)

gitlab-ci:
	+$(MAKE) -k CMAKE_BUILD_TYPE=Release memcheck sanitize coverage
	
memcheck: build
	cd $(B) && $(CTEST) -T memcheck $(CTESTFLAGS)

sanitize:
	+$(MAKE) B=$(B)_sanitize CMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer" test

doxygen:
	doxygen ./doc/Doxyfile
	mv ./public/html/* ./public/

coverage:
	+$(MAKE) B=$(B)_coverage CMAKE_C_FLAGS="-fprofile-arcs -ftest-coverage -g" .coverage

.coverage: test
	gcovr -r . -e test -e _build $(B)

clean:
	if [ -e _build ]; then rm -r _build; fi

distclean: clean
	if [ -e public ]; then rm -r public; fi


# Makefile

.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables
MAKEFLAGS += --warn-undefined-variables

CMAKE_BUILD_TYPE ?= Debug
# CMAKE_BUILD_TYPE ?= Release
 
B = _build/$(CMAKE_BUILD_TYPE)

NICE += $(shell hash nice 2>/dev/null >/dev/null && echo nice)
NICE += $(shell hash ionice 2>/dev/null >/dev/null && echo ionice)

CTEST := $(NICE) ctest
CTESTFLAGS += --output-on-failure # --verbose --rerun-failed

CMAKE := $(NICE) cmake
CMAKEFLAGS += -S test
CMAKEFLAGS += $(shell hash ninja 2>/dev/null >/dev/null && echo -GNinja)
CMAKEFLAGS += -D CMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
CMAKEFLAGS += -D CMAKE_RUNTIME_OUTPUT_DIRECTORY=bin
CMAKEFLAGS += -D CMAKE_LIBRARY_OUTPUT_DIRECTORY=lib
CMAKEFLAGS += -D CMAKE_EXECUTABLE_SUFFIX=.out
CMAKEFLAGS += -D CMAKE_VERBOSE_MAKEFILE=ON
 
all: test

build:
	$(CMAKE) -B $(B) $(CMAKEFLAGS)
	$(CMAKE) --build $(B)
	
test: build
	cd $(B) && $(CTEST) $(CTESTFLAGS) # -R yio_test_custom_callback_2

gitlab-ci:
	+$(MAKE) CMAKE_BUILD_TYPE=Release memcheck
	+$(MAKE) CMAKE_BUILD_TYPE=Release sanitize
	
memcheck: build
	cd $(B) && $(CTEST) -j $(shell nproc) -T memcheck $(CTESTFLAGS)

sanitize:
	$(CMAKE) -B $(B)/sanitize $(CMAKEFLAGS) -D "CMAKE_C_FLAGS=-fsanitize=address -fno-omit-frame-pointer"
	$(CMAKE) --build $(B)/sanitize
	cd $(B)/sanitize && $(CTEST) -j $(shell nproc) $(CTESTFLAGS)

doxygen:
	# rm -fr ./public
	doxygen ./doc/Doxyfile

clean:
	rm -vrf _build


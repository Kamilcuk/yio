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
NICE += $(shell hash ionice 2>/dev/null >/dev/null && ionice --version 2>&1 | grep -q util-linux && echo ionice)

CTEST := $(NICE) ctest
CTESTFLAGS += --output-on-failure 
CTESTFLAGS += -j $(shell nproc) # --verbose --rerun-failed

CMAKE_C_FLAGS ?= 

BUILD_TESTING ?= ON

CMAKE := $(NICE) cmake
CMAKEFLAGS += -S .
CMAKEFLAGS_GENERATOR ?= $(shell hash ninja 2>/dev/null >/dev/null && echo -GNinja)
CMAKEFLAGS += $(CMAKEFLAGS_GENERATOR)
CMAKEFLAGS += -D CMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
CMAKEFLAGS += -D CMAKE_VERBOSE_MAKEFILE=ON
CMAKEFLAGS += -D CMAKE_C_FLAGS="$(CMAKE_C_FLAGS)"
CMAKEFLAGS += -D BUILD_TESTING=$(BUILD_TESTING)

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

gitlab-ci: export CMAKE_BUILD_TYPE=Release
gitlab-ci:
	@+$(MAKE) -k memcheck sanitize test_project coverage gitlab_cdash

# Tests ######################################

memcheck: build
	cd $(B) && $(CTEST) -T memcheck $(CTESTFLAGS) -LE nomemcheck

sanitize: export CMAKE_C_FLAGS = -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=pointer-compare -fno-omit-frame-pointer -fstack-protector-all -fstack-clash-protection -fcf-protection
sanitize: export B_SUFFIX = _sanitize
sanitize:
	@+$(MAKE) test

.PHONY: coverage
coverage: export CMAKE_C_FLAGS = --coverage -g
coverage: export B_SUFFIX = _coverage
coverage:
	@+$(MAKE) .coverage
.coverage: test
	gcovr -r . -e test -e _build $(B)

# Doxygen #####################################
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

# cdash ##########################
CDASHROOT   = _build/cdash
CDASHSOURCE = $(CDASHROOT)/source
CDASHBUILD  = $(CDASHROOT)/_build
cdash: export CDASH_ARGS = -D MODEL=Experimental
cdash: .cdash ;
gitlab_cdash: export CDASH_ARGS = -D MODEL=Continous -D DO_SUBMIT=ON
gitlab_cdash: .cdash ;
cdash_submit: export CDASH_ARGS = -D MODEL=Experimental -D DO_SUBMIT=ON
cdash_submit: .cdash ;
.cdash: $(CDASHSOURCE)
	cd $(CDASHSOURCE) && ctest \
		$(CDASH_ARGS) \
		-D CTEST_BINARY_DIRECTORY=$(PWD)/$(CDASHBUILD) \
		-D CTEST_SOURCE_DIRECTORY=$(PWD)/$(CDASHSOURCE) \
		-S ./dashboard_chors.cmake -V
# create a separate source directory for cdash
# so that ctest_update() doesn't overwrite my files!
$(CDASHSOURCE): $(shell git ls-files . --exclude-standard --others --cached)
	# rm -rf $(CDASHSOURCE)
	mkdir -p $(CDASHSOURCE)
	{ echo .git; git ls-files . --exclude-standard --others --cached; } | \
	if hash rsync >/dev/null 2>&1; then \
		rsync --files-from=/dev/stdin -av . $(CDASHSOURCE) ; \
	else \
		xargs -t -d'\n' cp --parents -a -t $(CDASHSOURCE) ; \
	fi
		
# test building cmake project ##########################
test_project: B = _build/test_project
test_project: export YIODIR = $(PWD)/$(B)/testinstall
test_project: build
	$(CMAKE) -B $(B) $(CMAKEFLAGS) -D CMAKE_INSTALL_PREFIX=$(B)/testinstall
	$(CMAKE) --build $(B) --target all
	$(CMAKE) --build $(B) --target install
	$(MAKE) -C test/cmake_example
	$(CMAKE) --build $(B) --target uninstall
	
# standard ################################################
clean:
	if [ -e _build ]; then rm -r _build; fi

distclean: clean
	if [ -e public ]; then rm -r public; fi

install: build
	$(CMAKE) --build $(B) --target install
	
uninstall: build
	$(CMAKE) --build $(B) --target yio_uninstall

.PHONY: all $(MAKECMDGOALS)



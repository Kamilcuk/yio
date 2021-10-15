# Makefile

-include local_makefile.mk

SHELL = bash

MAKEFLAGS += -rR --no-print-directory
.SUFFIXES:
.NOTPARALLEL:

# check if we have nice
NICE += $(shell hash nice >/dev/null 2>&1 && echo nice)
# check if we have ionice from util-linux
NICE += $(shell hash ionice >/dev/null 2>&1 && echo ionice -c 3)
# Protect against non-existent sudo command
SUDO += $(shell hash sudo >/dev/null 2>&1 && echo sudo)
STDBUF = $(shell hash stdbuf >/dev/null 2>&1 && echo stdbuf -oL -eL)

HELP_VAR +=~ NPROC - Number of cores to use
NPROC = $(shell echo $$(( $$(grep -c processor /proc/cpuinfo) * 100 / 75)) )

###############################################################################

HELP_VAR +=~ CMAKE_BUILD_TYPE
CMAKE_BUILD_TYPE ?= Debug

HELP_VAR +=~ SYSTEM - {,arm,arm2,sdcc}
SYSTEM ?=

HELP_VAR +=~ MODE - {,coverage,sanitize}
MODE ?=

HELP_VAR +=~ CMAKE_C_FLAGS
CMAKE_C_FLAGS ?=

HELP_VAR +=~ BUILD_TESTING
BUILD_TESTING ?= ON

HELP_VAR +=~ CTESTFLAGS
CTESTFLAGS ?=

HELP_VAR +=~ LINT - set to 1 if want to run CPPLINT
LINT ?=
ifeq ($(LINT),1)
export YIO_CPPLINT=1
export YIO_CLANG_TIDY=1
endif

HELP_VAR +=~ R - Pass option to ctest -R
R ?=

###############################################################################

ifeq ($(SYSTEM),)
else ifeq ($(SYSTEM),arm)
CMAKEFLAGS += -DCMAKE_TOOLCHAIN_FILE=$(PWD)/cmake/Toolchain/arm-none-eabi-gcc.cmake
else ifeq ($(SYSTEM),arm2)
CMAKEFLAGS += -DCMAKE_TOOLCHAIN_FILE=$(PWD)/cmake/Toolchain/arm-none-eabi-gcc.cmake
CMAKE_C_FLAGS += -mthumb -march=armv7e-m -mfloat-abi=soft -g -Os -ffunction-sections -fdata-sections -flto
else ifeq ($(SYSTEM),sdcc)
CMAKEFLAGS += -DCMAKE_TOOLCHAIN_FILE=$(PWD)/cmake/Toolchain/sdcc.cmake
else
$(error SYSTEM - invalid value)
endif

ifeq ($(MODE),)
else ifeq ($(MODE),coverage)
CMAKE_C_FLAGS += -g --coverage -fprofile-abs-path
else ifeq ($(MODE),sanitize)
CMAKE_C_FLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=pointer-compare -fno-omit-frame-pointer -fstack-protector-all -fstack-clash-protection -fcf-protection
else
$(error MODE - invalid value)
endif

###############################################################################

# Build dir name
_BCCNAME =
ifdef CC
ifeq ($(findstring .$(CC).,.clang.icc.),.$(CC).)
_BCCNAME = $(CC)
endif
endif
ISALPINE ?= $(shell grep -q alpine /etc/os-release 2>/dev/null && echo 1)
ifeq ($(ISALPINE),1)
_BCCNAME = alpine
endif
_BNAME = $(_BCCNAME)$(SYSTEM)$(CMAKE_BUILD_TYPE)$(shell MODE=$(MODE) && echo $${MODE^})
HELP_VAR +=~ B - Build directory location
B ?= _build/$(_BNAME)

# Default cmake and other flags
CMAKE_BUILD_PARALLEL_LEVEL ?= $(NPROF)
CMAKE = $(NICE) cmake
CMAKEFLAGS += -S.
ifeq ($(shell hash ninja 2>&1),)
MAKEFLAGS += --warn-undefined-variables
CMAKEFLAGS += -GNinja
else
export CMAKE_BUILD_PARALLEL_LEVEL=$(CMAKE_BUILD_PARALLEL_LEVEL)
endif
ifneq ($(shell cmake --help | grep log-level),)
CMAKEFLAGS_INIT += --log-level=TRACE
endif
CMAKEFLAGS_INIT += -DYIO_DEV=1
CMAKEFLAGS += $(CMAKEFLAGS_INIT)
CMAKEFLAGS += -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
CMAKEFLAGS += $(if $(value CMAKE_C_FLAGS),-DCMAKE_C_FLAGS="$(CMAKE_C_FLAGS)")
CMAKEFLAGS += -DBUILD_TESTING=$(BUILD_TESTING)
CMAKEFLAGS += --warn-uninitialized

SED_FIX_PATHS = sed -u 's@^[^ ]*/gen/@src/@; s@^\.\./\.\./test@test@'
GEN_TO_SRC = 2> >($(SED_FIX_PATHS) >&2) > >($(SED_FIX_PATHS))

BUILDFLAGS ?=
CONFIGFLAGS ?=
VERBOSE ?= 0

ifeq ($(VERBOSE),1)
BUILDFLAGS += --verbose
CTESTFLAGS += -V
endif

TESTFLAGS ?=
CTESTFLAGS += $(if $(value R),-j 0 -R "$(R)")

###############################################################################

all: help

###############################################################################
# Generic configure+build+test targets

HELP +=~ configure - Configure the project
configure:
	$(CMAKE) -B$(B) $(CMAKEFLAGS) $(CONFIGFLAGS)

HELP +=~ .build_% - Generic target build
.build_%: unexport MAKEFLAGS
.build_%: configure
	$(CMAKE) --build $(B) --target $* $(BUILDFLAGS) <&-

HELP +=~ build_gen - Only generate the files from m4 preprocessor
build_gen: .build_yio_gen

HELP +=~ build_yio - Build the yio target
build_yio: .build_yio

HELP +=~ build - Build the project
build: .build_all

HELP +=~ test - Run tests using ctest
test: build testonly

testonly:
	ulimit -c 0 ; ( cd $(B) && $(NICE) ctest --output-on-failure -j $(NPROC) $(CTESTFLAGS) $(TESTFLAGS) )

HELP +=~ valgrind - Run tests under valgrind
valgrind: CTESTFLAGS += -LE nomemcheck -D ExperimentalMemCheck
valgrind: test

HELP +=~ testone_% - Build and test one specific target
testone_%:
	$(MAKE) .build_$* testonly CTESTFLAGS="-j 0 -R '^$*\$$\$$' -V"

lint:
	$(MAKE) LINT=1 CONFIGFLAGS='-D_yIO_HAS_FLOATd32=0 -D_yIO_HAS_FLOATd64=0 -D_yIO_HAS_FLOATd128=0' B=.cache/lint build

###############################################################################

HELP +=~ cicd - Run all tests before commit to be sure it works
cicd: export CMAKE_BUILD_TYPE=Release
cicd:
	$(MAKE) valgrind
	$(MAKE) test_project
	$(MAKE) test MODE=sanitize
	CC=clang $(MAKE) test
	$(MAKE) SYSTEM=arm test
	$(MAKE) gitlab_pages

HELP +=~ debug - Build the project in debug mode
debug: export CMAKE_BUILD_TYPE=Debug
debug:
	@+$(MAKE) build

HELP +=~ release - Build the project in release mode
release: export CMAKE_BUILD_TYPE=Release
release:
	@+$(MAKE) build

HELP +=~ release_test - Build and test the project in release mode
release_test: export CMAKE_BUILD_TYPE=Release
release_test:
	@+$(MAKE) test

# Exotic Targets ##################################################

coverage:
	$(MAKE) _coverage_in MODE=coverage
_coverage_in: CTESTFLAGS += -LE nomemcheck
_coverage_in: CMAKEFLAGS_INIT =
_coverage_in: CMAKEFLAGS += -UYIO_DEV -DYIO_BUILD_TESTING=1
#_coverage_in: CMAKE_BUILD_TYPE=Release
_coverage_in: B = _build/coverage
_coverage_in: test _coverage_gen
_coverage_gen: MODE=coverage
_coverage_gen:
	gcovr --print-summary --txt - --xml ./_build/cobertura-coverage.xml --json ./coverage.json --filter "$(B)/gen" --filter "src" -r . "$(B)"

###############################################################################
# cppcheck

CPPCHECK_FLAGS_INIT = \
	--project=_build/Release/compile_commands.json \
	-j $(shell nproc) \
	-U__DOXYGEN__ \
	-U__CDT_PARSER__ \
	-UYIO_REALLY_CDT_PARSER \
	-UNDEBUG \
	-I${linuxincdir}/include \
	-I/usr/local/include \
	-I${linuxincdir}/include-fixed \
	-I/usr/include \
	-I${PWD}/gen \
	--suppress=unmatchedSuppression \
	--suppress=unreadVariable \
	--suppress=unusedFunction \
	--library=posix \
	--report-progress  \
	-x c
CPPCHECK_FLAGS = --quiet --enable=all
HELP +=~ cppcheck Run cppcheck
cppcheck: linuxincdir = $(firstword $(wildcard /usr/lib/gcc/x86_64-pc-linux-gnu/*/))
cppcheck: release
	nice cppcheck ${CPPCHECK_FLAGS_INIT} ${CPPCHECK_FLAGS}

###############################################################################
# ctags

ctags:
	git ls-files -z --exclude-standard --others --cached test/ src/ | \
	xargs -0 nice ctags --recurse --append --extras=+q --fields=+aimS --c-kinds=+p --c++-kinds=+p

ctags_gen:
	find gen/ test/ -type f -print0 | \
	xargs -0 nice ctags --recurse --append --extras=+q --fields=+aimS --c-kinds=+p --c++-kinds=+p

###############################################################################

test_alpine: USER  = $(shell id -u)
test_alpine: GROUP = $(shell id -g)
test_alpine:
	@mkdir -p _build/alpine/emptycontext
	docker build -t yio_alpine_builder -f alpine_builder.Dockerfile _build/alpine/emptycontext
	docker run -ti --rm -v $(CURDIR):/mnt -w /mnt -u ${USER}:${GROUP} \
		yio_alpine_builder make test B=_build/alpine

###############################################################################
# Gitlab

HELP +=~ gitlab_pages - Generate gitlab pages
gitlab_pages: doxygen
	mkdir -p public
	./scripts/create_public_index.sh

gitlab_metrics:
	./scripts/metrics.sh _build/Release | tee metrics.txt

###############################################################################
# Doxygen

HELP +=~ doxygen - Generates doxygen html documentation in pages/doxygen
.PHONY: doxygen
doxygen: build_gen
	# Copy source file into one directory.
	./scripts/syncdir.sh ./gen/* _build/doxygen/input/
	# Extract m4 documentation
	mkdir -p _build/doxygen/input/m4/
	find m4/ -type f -exec cat {} + | \
		sed -n '\~.*\(/\*\*\)~{ s//\1/; : incomment; \~\(\*/\).*~{ s//\1/; p; b; }; p; n; b incomment; }; /^#/p' \
		> _build/doxygen/input/m4/m4.h
	# Generate doxygen documentation.
	mkdir -p _build/doxygen/output
	doxygen doc/Doxyfile
	# Generate public/doxygen directory.
	./scripts/syncdir.sh _build/doxygen/output/html/* public/doxygen/
.PHONY: doxygen_open
doxygen_open: doxygen
	xdg-open public/doxygen/index.html

###############################################################################
# test building cmake project

HELP +=~ test_project - Test sample cmake project
test_project: CMAKEFLAGS += -D BUILD_TESTING=NO -D CMAKE_INSTALL_PREFIX=$(B)/testinstall
test_project: build
	$(CMAKE) --build $(B) --target install
	MAKEFLAGS= $(MAKE) -C test/cmake_proj_tests/cmake_example \
		YIODIR=$(PWD)/$(B)/testinstall \
		B=$(PWD)/_build/test_project
	$(CMAKE) --build $(B) --target yio_uninstall

HELP +=~ test_project_add_subdirectory - Test cmake project that uses add_subdirectory
test_project_add_subdirectory:
	MAKEFLAGS= $(MAKE) -C test/cmake_proj_tests/cmake_subproject \
		YIODIR=$(PWD)/$(B)/testinstall \
		B=$(PWD)/_build/test_project_add_subdirectory

HELP +=~ test_project_add_subdirectory - Install globally cmake project and tests add_subdirectory
test_project_install_add_subdirectory: CMAKEFLAGS += -D BUILD_TESTING=NO -D CMAKE_INSTALL_PREFIX=/usr/local/
test_project_install_add_subdirectory: build
	sudo $(CMAKE) --build $(B) --target install
	MAKEFLAGS= $(MAKE) -C test/cmake_proj_tests/cmake_example \
		B=$(PWD)/_build/test_project_install_add_subdirectory
	sudo $(CMAKE) --build $(B) --target yio_uninstall

HELP +=~ test_project_no_install - Test cmake project using system wide yio installation
test_project_no_install: clean_test_project
	MAKEFLAGS= $(MAKE) -C test/cmake_proj_tests/cmake_example \
		B=$(PWD)/_build/test_project_no_install

HELP +=~ clean_test_project - Cleans test_projects
clean_test_project:
	rm -rf _build/test_project*

###############################################################################
# standard

HELP +=~ install - install project
install: export CMAKE_BUILD_TYPE=Release
install: .build_install

HELP +=~ clean - Remove specific directory
clean:
	rm -fr $(B)

HELP +=~ distclean - Removes whole _build and public directories
distclean:
	rm -fr ./_build ./public

###############################################################################
# Help library

define HELP_DESC
Usage: make [VAR=val] target...
endef
HELP_TGTS = $(subst ~,,$(filter ~%,$(subst ~ ,~,$(HELP))))
.PHONY: $(HELP_TGTS)
# All targets added to HELP are phony
HELP +=~ help - Prints this text
help: export HELP_DESC:=$(HELP_DESC)
help: export HELP:=$(HELP)
help: export HELP_VAR:=$(HELP_VAR)
define HELP_PARSE
	tr '~' '\n' | \
	sort | \
	sed '/^[[:space:]]*$$/d; s/[[:space:]]*\([^ ]*\) *-\? */  \1~/' | \
	if hash column 2>/dev/null; then \
		if column --version 2>&1 | grep -q util-linux; then \
			column -t -s "~" -o "  "; \
		else \
			column -t -s "~"; \
		fi; \
	else \
		sed 's/|/   - /'; \
	fi
endef
HELP_VARIABLES = $(subst ~,,$(filter ~%,$(subst ~ ,~,$(HELP_VAR))))
help:
	@echo "$$HELP_DESC"
	@echo
	@echo "Targets:"
	@echo "$$HELP" | $(HELP_PARSE)
	@$(if $(value HELP_VAR), echo)
	@$(if $(value HELP_VAR), echo "Options:")
	@$(if $(value HELP_VAR),\
	printf "Default: %s\n" $(foreach v,$(HELP_VARIABLES),"$(v)=$($(v))") | \
	paste -d' ' <(tr '~' '\n' <<<"$$HELP_VAR" | sed 1d) - | $(HELP_PARSE) )


###############################################################################

.PHONY: eclipse all


# Makefile

define NL


endef

SHELL = bash

.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-print-directory
export MAKEFLAGS

CMAKE_BUILD_TYPE ?= Debug
export CMAKE_BUILD_TYPE
 
SYSTEM ?=
export SYSTEM

B_SUFFIX ?=
B ?= _build/$(SYSTEM)$(CMAKE_BUILD_TYPE)$(B_SUFFIX)

# check if we have nice
NICE += $(shell hash nice 2>/dev/null >/dev/null && echo nice)
# check if we have ionice from util-linux
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
CMAKEFLAGS += -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
CMAKEFLAGS += -DCMAKE_VERBOSE_MAKEFILE=ON
ifneq ($(CMAKE_C_FLAGS),)
CMAKEFLAGS += -DCMAKE_C_FLAGS="$(CMAKE_C_FLAGS)"
endif
CMAKEFLAGS += -DBUILD_TESTING=$(BUILD_TESTING)


ifeq ($(SYSTEM),ARM10E)
ARM10E_CMAKEFLAGS += -DCMAKE_TOOLCHAIN_FILE=$(PWD)/cmake/toolchain-arm-none-eabi.cmake
ARM10E_CMAKEFLAGS += -DCMAKE_CROSSCOMPILING_EMULATOR=$(PWD)/scripts/cmake_crosscompiling_emulator_arm_none_gdb.sh

CMAKEFLAGS += $(ARM10E_CMAKEFLAGS)

CDASHFLAGS += -DSYSTEM=ARM10E
CDASHFLAGS += -DWITH_SANITIZE=OFF
CDASHFLAGS += -DWITH_COVERAGE=OFF
CDASHFLAGS += -DWITH_SANITIZE=OFF
CDASHFLAGS += $(ARM10E_CMAKEFLAGS)
endif

SED_FIX_PATHS = sed -u 's@^[^ ]*/gen/@src/@; s@^\.\./\.\./test@test@'
GEN_TO_SRC = 2> >($(SED_FIX_PATHS) >&2) > >($(SED_FIX_PATHS))
STDBUF = $(shell hash stdbuf 2>/dev/null >/dev/null && echo stdbuf -oL -eL) 

# Targets ###################################################################

all: usage 

USAGE +=~ eclipse - Target run from IDE
eclipse: configure build test

# Generic configure+build+test targets

USAGE +=~ configure - Configure the project
configure:
	@echo $(CMAKE) -B $(B) $(CMAKEFLAGS)
	@$(CMAKE) -B $(B) $(CMAKEFLAGS)
	@ln -nvfs $(B)/src/gen gen ||:

USAGE +=~ .build_% - Generic target build
.build_%: unexport MAKEFLAGS
.build_%: configure
	@echo $(CMAKE) --build $(B) --target $*
	@$(STDBUF) $(CMAKE) --build $(B) --target $* $(GEN_TO_SRC)

USAGE +=~ build_gen - Only generate the files from m4 preprocessor
build_gen: .build__yio_gen

USAGE +=~ build_yio - Build the yio target
build_yio: .build_yio
	
USAGE +=~ build - Build the project
build: .build_all

USAGE +=~ test - Run tests using ctest
test: build
	cd $(B) && $(CTEST) $(CTESTFLAGS)

USAGE +=~ testall - Runs all known builds and tests
testall: test memcheck sanitize test_project coverage

USAGE +=~ cmake-gui - Runs cmake-gui
cmake-gui:
	cmake-gui -B $(B) $(CMAKEFLAGS)
	
USAGE +=~ ccmake - Runs ccmake
ccmake:
	ccmake -B $(B) $(CMAKEFLAGS)

# Gitlab ####################################

USAGE +=~ .gitlab_% - Internal gitlab target for setting up environment
.gitlab_%: export CMAKE_BUILD_TYPE = Release
.gitlab_%: export CDASHFLAGS += -DMODEL=Continous -DDO_SUBMIT=ON
.gitlab_%:
	@+$(MAKE) $*

USAGE +=~ gitlab_test - Test everything there to test
gitlab_test: .gitlab_testall
	
USAGE +=~ gitlab_cdash - Run cdash
gitlab_cdash: .gitlab_cdash

USAGE +=~ gitlab_clang_test - Run all tests but with clang
gitlab_clang_test: export CMAKEFLAGS += -DCMAKE_C_COMPILER=clang
gitlab_clang_test: .gitlab_testall

USAGE +=~ gitlab_arm_testall - Run all possible tests on arm
gitlab_arm_testall: export SYSTEM = ARM10E
gitlab_arm_testall: .gitlab_test

USAGE +=~ gitlab_arm_cdash - Run cdash with all possible tests on arm 
gitlab_arm_cdash: export SYSTEM = ARM10E
gitlab_arm_cdash: .gitlab_cdash

# Tests ######################################

USAGE +=~ memcheck - Run tests with valgrind under ctest
memcheck: build
	cd $(B) && $(CTEST) -T memcheck $(CTESTFLAGS) -LE nomemcheck

USAGE +=~ sanitize - Run tests with super ultra sanitize options
sanitize: export CMAKE_C_FLAGS += -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=pointer-compare -fno-omit-frame-pointer -fstack-protector-all -fstack-clash-protection -fcf-protection
sanitize: export B_SUFFIX = _sanitize
sanitize:
	@+$(MAKE) test

USAGE +=~ coverage - Generate coverage report
.PHONY: coverage
coverage: export CMAKE_C_FLAGS = --coverage -g
coverage: export B_SUFFIX = _coverage
coverage:
	@+$(MAKE) .coverage
.coverage: test
	gcovr -r . -e test -e _build $(B)

# Doxygen #####################################
_build/doxygen:
	mkdir -p _build/doxygen/
_build/Doxyfile: doc/Doxyfile build_gen
	sed \
		-e "/STRIP_FROM_PATH/s@<ABSOLUTE_GEN_SEE_MAKEFILE>@$$(readlink -f ./gen)@" \
		doc/Doxyfile > _build/Doxyfile
USAGE +=~ doxygen - Generates doxygen html documentation in pages/doxygen
.PHONY: doxygen
doxygen: build_gen _build/Doxyfile
	doxygen _build/Doxyfile
	rm -fr public/doxygen
	mkdir -p public
	cp -a _build/doxygen/html/ public/doxygen/ 

# cdash ##########################
CDASHROOT   = _build/$(SYSTEM)cdash
CDASHSOURCE = $(CDASHROOT)/source
CDASHBUILD  = $(CDASHROOT)/_build

USAGE +=~ cdash - Runs cdash locally without submitting results
cdash: CDASHFLAGS ?=
cdash: .cdash

USAGE +=~ cdash_cubmit - Runs cdash locally and submit results
cdash_submit: CDASHFLAGS ?= -DDO_SUBMIT=ON
cdash_submit: .cdash

.cdash: $(CDASHSOURCE)
	cd $(CDASHSOURCE) && ctest \
		$(CDASHFLAGS) \
		-DCTEST_BINARY_DIRECTORY=$(PWD)/$(CDASHBUILD) \
		-DCTEST_SOURCE_DIRECTORY=$(PWD)/$(CDASHSOURCE) \
		-S ./dashboard_chors.cmake -V

# create a separate source directory for cdash
# so that ctest_update() doesn't overwrite my files!
$(CDASHSOURCE): $(shell git ls-files . --exclude-standard --others --cached)
	# rm -rf $(CDASHSOURCE)
	./scripts/copy_git_files_only.sh $(CDASHSOURCE)

# test building cmake project ##########################

USAGE +=~ test_project - Test sample cmake project
test_project: export B = _build/test_project
test_project: build
	$(CMAKE) -B $(B) $(CMAKEFLAGS) -D CMAKE_INSTALL_PREFIX=$(B)/testinstall
	$(CMAKE) --build $(B) --target all
	$(CMAKE) --build $(B) --target install
	$(MAKE) YIODIR=$(PWD)/$(B)/testinstall -C test/cmake_example
	$(CMAKE) --build $(B) --target yio_uninstall
	
# standard ################################################

USAGE +=~ clean - Remove _build directory
clean:
	if [ -e _build ]; then rm -r _build; fi

USAGE +=~ distclean - Removes _build and public also
distclean: clean
	if [ -e public ]; then rm -r public; fi

USAGE +=~ install - install project
install: .build_install

USAGE +=~ uninstall - uninstall files
uninstall: .build_yio_uninstall

# Usage #######################################################

export TGTSUSAGES=$(subst ~, $(NL), $(USAGE))
define USAGESTRING
Usage: make VAR=... target

Variables you can set:
  SYSTEM            - Empty means host system, ARM10E means use arm-none-eabi
  CMAKEFLAGS        - Arguments passed to cmake in configure step
  CDASHFLAGS        - Arguments passed to cdash
  CTESTFLAGS        - Arguments passed to ctest on the rare occasion

Variables passed to dashboard_chors.cmake that you can set with CDASHFLAGS=...:
  MODEL           - The ctest model: Experimental(default), Continous, Nightly
  WITH_UPDATE     - Checkout the project. Default OFF
  WITH_MEMCHECK   - Do memcheck step. Default ON
  WITH_COVERAGE   - Run and test coverage build. Default ON
  WITH_SANITIZE   - Run and test sanitize build. Default ON
  WITH_SUBMIT     - Submit to dashboard. Default OFF
    
Variables set internally that you should not set:
  B                 - Build directory, ./_build
  B_SUFFIX          - Suffix to be added to build directory
  CMAKE_BUILD_TYPE

Targets:
endef
export USAGESTRING
usage:
	@./scripts/gen_make_help.sh "$$USAGESTRING" "$$TGTSUSAGES"

.PHONY: all $(MAKECMDGOALS)



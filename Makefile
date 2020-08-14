# Makefile

include scripts/makefile_lib.mk
-include local_makefile.mk

SHELL = bash

.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables
MAKEFLAGS += --no-print-directory
export CDASH_KARTA_DYZIO_PL_PASSWORD

CMAKE_BUILD_TYPE ?= Debug
export CMAKE_BUILD_TYPE

SYSTEM ?=
export SYSTEM

B_SUFFIX ?=
B ?= _build/$(SYSTEM)$(CMAKE_BUILD_TYPE)$(B_SUFFIX)

# check if we have nice
NICE += $(shell hash nice >/dev/null 2>&1 && echo nice)
# check if we have ionice from util-linux
NICE += $(shell hash ionice >/dev/null 2>&1 && ionice --version 2>&1 | grep -q util-linux && echo ionice)
# Protect against non-existent sudo command
SUDO += $(shell hash sudo >/dev/null 2>&1 && echo sudo)

CTESTFLAGS_INIT += --output-on-failure
CTESTFLAGS += -j $(shell nproc) # --verbose --rerun-failed
CTEST = ulimit -c 0; $(NICE) ctest $(CTESTFLAGS_INIT)

CMAKE_C_FLAGS ?=

CDASHFLAGS ?=

BUILD_TESTING ?= ON

CMAKE = $(NICE) cmake
CMAKEFLAGS_INIT += -S.
ifeq ($(shell hash ninja 2>&1),)
MAKEFLAGS += --warn-undefined-variables
CMAKEFLAGS_INIT += -GNinja
else
NPROC = $(shell grep -c processor /proc/cpuinfo)
export CMAKE_BUILD_PARALLEL_LEVEL=$(NPROC)
endif
ifneq ($(shell cmake --help | grep log-level),)
CMAKEFLAGS_INIT += --log-level=TRACE
endif
CMAKEFLAGS_INIT += -DYIO_DEV=1
CMAKEFLAGS += $(CMAKEFLAGS_INIT)
CMAKEFLAGS += -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
ifneq ($(CMAKE_C_FLAGS),)
CMAKEFLAGS += -DCMAKE_C_FLAGS="$(CMAKE_C_FLAGS)"
endif
CMAKEFLAGS += -DBUILD_TESTING=$(BUILD_TESTING)
CMAKEFLAGS += --warn-uninitialized
CMAKEADDFLAGS ?=
CMAKEFLAGS += $(CMAKEADDFLAGS)

SED_FIX_PATHS = sed -u 's@^[^ ]*/gen/@src/@; s@^\.\./\.\./test@test@'
GEN_TO_SRC = 2> >($(SED_FIX_PATHS) >&2) > >($(SED_FIX_PATHS))
STDBUF = $(shell hash stdbuf >/dev/null 2>&1 && echo stdbuf -oL -eL)

# Targets ###################################################################

all: usage

# Generic configure+build+test targets

USAGE +=~ configure - Configure the project
configure:
	$(CMAKE) -B$(B) $(CMAKEFLAGS)

USAGE +=~ .build_% - Generic target build
.build_%: unexport MAKEFLAGS
.build_%: configure
	$(CMAKE) --build $(B) --target $*

USAGE +=~ build_gen - Only generate the files from m4 preprocessor
build_gen: .build_yio_gen

USAGE +=~ build_yio - Build the yio target
build_yio: .build_yio

USAGE +=~ build - Build the project
build: .build_all

USAGE +=~ test - Run tests using ctest
test: build
	cd $(B) && $(CTEST) $(CTESTFLAGS)

test_before_commit:
	$(MAKE) clean
	$(MAKE) test
	$(MAKE) test_project_add_subdirectory
	$(MAKE) test_project_install_add_subdirectory
	$(MAKE) cdash_gcc
	$(MAKE) cdash_arm
	$(MAKE) -C pkg clean
	$(MAKE) pages_repos

USAGE +=~ test_R_% - Run tests matching regular expression
test_R_: test
test_R_%: build
	cd $(B) && $(CTEST) $(CTESTFLAGS) -R "$*"

USAGE +=~ test_L_% - Run tests with labels matching regex
test_L_: test
test_L_%: build
	cd $(B) && $(CTEST) $(CTESTFLAGS) -L "$*"

USAGE +=~ test_rerun_failed - Run tests with --rerun-failed
test_rerun_failed: CTESTFLAGS_INIT += --rerun-failed
test_rerun_failed: test

USAGE +=~ debug - Build the project in debug mode
debug: export CMAKE_BUILD_TYPE=Debug
debug:
	@+$(MAKE) build

USAGE +=~ release - Build the project in release mode
release: export CMAKE_BUILD_TYPE=Release
release:
	@+$(MAKE) build

USAGE +=~ release_test - Build and test the project in release mode
release_test: export CMAKE_BUILD_TYPE=Release
release_test:
	@+$(MAKE) test

USAGE +=~ cmake-gui - Runs cmake-gui
cmake-gui:
	cmake-gui -B $(B) $(CMAKEFLAGS)

USAGE +=~ ccmake - Runs ccmake
ccmake:
	ccmake -B $(B) $(CMAKEFLAGS)

# Exotic Targets ##################################################

ifeq ($(SYSTEM),arm)
CMAKEFLAGS += -DCMAKE_TOOLCHAIN_FILE=$(PWD)/cmake/Toolchain/arm-none-eabi-gcc.cmake
endif

USAGE +=~ arm - Test building with arm-none-eabi
arm: ; @$(MAKE) SYSTEM=arm build

USAGE +=~ test_arm - Run tests with arm-none-eabi
test_arm: ; @$(MAKE) SYSTEM=arm test

USAGE +=~ test_arm_R_% - Run tests matching regex on arm-none-eabi
test_arm_R_%: ; @$(MAKE) SYSTEM=arm test_R_$*

ifeq ($(SYSTEM),arm2)
CMAKEFLAGS += -DCMAKE_TOOLCHAIN_FILE=$(PWD)/cmake/Toolchain/arm-none-eabi-gcc.cmake
CMAKEFLAGS += -DCMAKE_C_FLAGS=" -mthumb -march=armv7e-m -mfloat-abi=soft -g -Os -ffunction-sections -fdata-sections -flto "
endif
test_arm2: ; $(MAKE) SYSTEM=arm2 test

define _sdcc_pic16
$1: B = _build/sdcc
$1: CMAKEFLAGS += -DCMAKE_TOOLCHAIN_FILE=$(PWD)/cmake/Toolchain/sdcc.cmake
endef
USAGE +=~ sdcc_pic16 - Tests building with sdcc_pic16
$(eval $(call _sdcc_pic16,sdcc_pic16))
sdcc_pic16: build


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
cppcheck: linuxincdir = $(firstword $(wildcard /usr/lib/gcc/x86_64-pc-linux-gnu/*/))
cppcheck: release
	nice cppcheck ${CPPCHECK_FLAGS_INIT} ${CPPCHECK_FLAGS}

ctags:
	git ls-files -z --exclude-standard --others --cached test/ src/ | \
	xargs -0 nice ctags --recurse --append --extras=+q --fields=+aimS --c-kinds=+p --c++-kinds=+p

ctags_gen:
	find gen/ test/ -type f -print0 | \
	xargs -0 nice ctags --recurse --append --extras=+q --fields=+aimS --c-kinds=+p --c++-kinds=+p

test_alpine:
	@mkdir -p _build/alpine/emptycontext
	docker build -t yio_alpine_builder -f alpine_builder.Dockerfile _build/alpine/emptycontext
	docker run -ti --rm -v $(CURDIR):/mnt -w /mnt -u ${USER}:${GROUP} \
		yio_alpine_builder make test B=_build/alpine

# Gitlab ####################################

define gitlab_cdash_decl_them

USAGE +=~ gitlab_$(1)_cdash - Test everything there to test
gitlab_$(1)_cdash: .gitlab_cdash_$(1)

endef # gitlab_cdash_decl_them
$(foreach i,gcc clang arm,$(eval $(call gitlab_cdash_decl_them,$(i))))

.gitlab_cdash_%: export override CDASHFLAGS := -DMODEL=Continous -DWITH_UPDATE=ON -DWITH_SUBMIT=ON $(CDASHFLAGS)
.gitlab_cdash_%: .cdash_% ;

USAGE +=~ gitlab_pages - Generate gitlab pages
gitlab_pages: doxygen
	mkdir -p public
	./scripts/badge_json_gen.sh > public/badge.json
	cat public/badge.json
	./scripts/create_public_index.sh

pages_repos:
	make -C pkg PREFIX=$(PWD)/public

gitlab_metrics:
	./scripts/metrics.sh _build/Release | tee metrics.txt

# cdash ##########################

CDASHFLAGS ?=

USAGE +=~ cdash - Shortcut to cdash_gcc
cdash: cdash_gcc

USAGE +=~ cdash_submit - Shortcut to cdash_gcc_submit
cdash_submit: cdash_gcc_submit

define cdash_decl_them

USAGE +=~ cdash_$(1)_submit - Runs cdash locally with $(1) and submits the results
cdash_$(1)_submit: export override CDASHFLAGS := -D WITH_SUBMIT=ON $(CDASHFLAGS)
cdash_$(1)_submit: .cdash_$(1) ;

USAGE +=~ cdash_$(1) - Runs cdash locally with $(1)
cdash_$(1): .cdash_$(1) ;

USAGE +=~ cdashnocopy_$(1) - Runs cdash locally with $(1) without copying the source
cdashnocopy_$(1): .cdashnocopy_$(1) ;

cdashnocopy_all: .cdashnocopy_$(1)

USAGE +=~ testcdash_$(1) - Runs cdashnocopy_$(1)
testcdash_$(1): export override CDASHFLAGS := -DWITH_COVERAGE=NO -DWITH_SANITIZE=NO -DWITH_SUBMIT=NO $(CDASHFLAGS)
testcdash_$(1): .cdashnocopy_$(1)

endef # cdash_decl_them
$(foreach i,gcc clang arm,$(eval $(call cdash_decl_them,$(i))))

USAGE +=~ testcdash_all - Runs cdashnocopy_all
testcdash_all: cdashnocopy_all ;

USAGE +=~ cdashnocopy_all - Runs all possible cdashnocopy_*
cdashnocopy_all: ;

# Runs cdash dashboard in current directory
.cdashnocopy_%:
	ctest -DDASHBOARD_MODE=$* \
		$(CDASHFLAGS) \
		-DWITH_UPDATE=NO -DWITH_SUBMIT=NO \
		-DCTEST_BINARY_DIRECTORY=_build/cdash/$* -DCTEST_SOURCE_DIRECTORY=. \
		-S ./dashboard.cmake -VV

CDASHROOT = $(PWD)/_build/cdash
# Copies the whole git tree to remote location and runs from there
.cdash_%:
	# Create a separate source directory for cdash.
	./scripts/copy_git_files_only.sh $(CDASHROOT)/source/$*
	# Run ctest in proper directory.
	cd $(CDASHROOT)/source/$* && ctest -DDASHBOARD_MODE=$* \
		$(CDASHFLAGS) \
		-DCTEST_BINARY_DIRECTORY=$(CDASHROOT)/build/$* \
		-DCTEST_SOURCE_DIRECTORY=$(CDASHROOT)/source/$* \
		-S ./dashboard.cmake -V

USAGE +=~ clean_cdash - Clean cdash related files
clean_cdash:
	rm -fr $(CDASHROOT)

# Doxygen #####################################

USAGE +=~ doxygen - Generates doxygen html documentation in pages/doxygen
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

# test building cmake project ##########################

TEST_PROJECT_B = _build/test_project_yio

USAGE +=~ test_project - Test sample cmake project
test_project:
	$(CMAKE) -B $(TEST_PROJECT_B) $(CMAKEFLAGS) -D BUILD_TESTING=NO -D CMAKE_INSTALL_PREFIX=$(TEST_PROJECT_B)/testinstall
	$(CMAKE) --build $(TEST_PROJECT_B) --target all
	$(CMAKE) --build $(TEST_PROJECT_B) --target install
	MAKEFLAGS= $(MAKE) -C test/cmake_proj_tests/cmake_example \
		YIODIR=$(PWD)/$(TEST_PROJECT_B)/testinstall \
		B=$(PWD)/_build/test_project
	$(CMAKE) --build $(TEST_PROJECT_B) --target yio_uninstall

USAGE +=~ test_project_add_subdirectory - Test cmake project that uses add_subdirectory
test_project_add_subdirectory:
	MAKEFLAGS= $(MAKE) -C test/cmake_proj_tests/cmake_subproject \
		YIODIR=$(PWD)/$(TEST_PROJECT_B)/testinstall \
		B=$(PWD)/_build/test_project_add_subdirectory

USAGE +=~ test_project_add_subdirectory - Install globally cmake project and tests add_subdirectory
test_project_install_add_subdirectory:
	$(CMAKE) -B $(TEST_PROJECT_B) $(CMAKEFLAGS) -D BUILD_TESTING=NO -D CMAKE_INSTALL_PREFIX=/usr/local/
	$(CMAKE) --build $(TEST_PROJECT_B) --target all
	sudo $(CMAKE) --build $(TEST_PROJECT_B) --target install
	MAKEFLAGS= $(MAKE) -C test/cmake_proj_tests/cmake_example \
		B=$(PWD)/_build/test_project_install_add_subdirectory
	sudo $(CMAKE) --build $(TEST_PROJECT_B) --target yio_uninstall

USAGE +=~ test_project_no_install - Test cmake project using system wide yio installation
test_project_no_install: clean_test_project
	MAKEFLAGS= $(MAKE) -C test/cmake_proj_tests/cmake_example \
		B=$(PWD)/_build/test_project_no_install

USAGE +=~ clean_test_project - Cleans test_projects
clean_test_project:
	rm -rf _build/test_project*

# standard ################################################

USAGE +=~ clean - Remove _build directory
clean:
	rm -fr _build

USAGE +=~ distclean - Removes _build and public also
distclean: clean
	rm -fr ./public
	$(MAKE) -C pkg/archlinux clean
	$(MAKE) -C pkg/apt clean

USAGE +=~ install - install project
install: export CMAKE_BUILD_TYPE=Release
install: .build_install

USAGE +=~ install_pkg - Automatically detect package manager and install package
ifneq (,$(wildcard /etc/arch-release))
install_pkg: install_archlinux
else ifneq (,$(wildcard /etc/debian_version))
install_pkg: install_deb
else
install_pkg:
	$(error Unhandled operating system)
endif

USAGE +=~ install_archlinux - Install the package on archlinux
install_archlinux:
	$(MAKE) -C pkg/archlinux yio
	$(SUDO) pacman -U pkg/archlinux/_build/build-yio/yio*.pkg.tar.*[^g]

USAGE +=~ install_archlinux_arm_none-eabi - Install the arm-none-eabi-yio package on archlinux
install_archlinux_arm_none_eabi:
	$(MAKE) -C pkg/archlinux yio-arm-none-eabi
	$(SUDO) pacman -S pkg/archlinux/_build/build-arm-none-eabi-yio/yio*.pkg.tar.*[^g]

USAGE +=~ install_deb - Install a deb package
install_deb: export CMAKE_BUILD_TYPE=Release
install_deb: build_yio
	cd _build/Release && cpack -G DEB
	$(SUDO) dpkg -i _build/Release/*.deb

USAGE +=~ uninstall - uninstall files
uninstall: export CMAKE_BUILD_TYPE=Release
uninstall: .build_yio_uninstall

# Usage #######################################################

export TGTSUSAGES=$(subst ~, $(NL), $(USAGE))
define USAGESTRING
Usage: make VAR=... target

Variables you can set:
  SYSTEM            - Empty means host system, armv4t means use arm-none-eabi
  CMAKEFLAGS        - Arguments passed to cmake in configure step
  CDASHFLAGS        - Arguments passed to cdash
  CTESTFLAGS        - Arguments passed to ctest on the rare occasion

Variables passed to dashboard_chors.cmake that you can set with CDASHFLAGS=...:
  DASHBOARD_MODE  - Mode to run for: gcc, clang or arm.
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
USAGE +=~ help - Prints this text
help: usage
USAGE +=~ usage - Prints this text
usage:
	@./scripts/gen_make_help.sh "$$USAGESTRING" "$$TGTSUSAGES"

# All targets added to USAGE are phony
.PHONY: $(TGTSUSAGES: -%=)
.PHONY: all


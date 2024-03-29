# Makefile

-include local_makefile.mk

SHELL = bash
MAKEFLAGS += -rR --no-print-directory --warn-undefined-variables
.SUFFIXES:
null :=
space := $(null) $(null)
define \n


endef
define nicecmd
$(patsubst #%,,$(strip $(subst %{\n},${space},${${1}})))
endef

# check if we have nice
NICE += $(shell hash nice >/dev/null 2>&1 && echo nice)
# check if we have ionice from util-linux
NICE += $(shell hash ionice >/dev/null 2>&1 && echo ionice -c 3)

HELP_VAR +=~ NPROC - Number of cores to use
NPROC = $(shell echo $$(( $$(grep -c processor /proc/cpuinfo) * 100 / 75)) )

###############################################################################

HELP_VAR +=~ CMAKE_BUILD_TYPE
CMAKE_BUILD_TYPE ?= Debug

HELP_VAR +=~ CMAKE_C_FLAGS
CMAKE_C_FLAGS ?=

HELP_VAR +=~ CTESTFLAGS
CTESTFLAGS ?=

HELP_VAR +=~ PRESET
PRESET ?= default

HELP_VAR +=~ LINT - set to 1 if want to run CPPLINT
LINT ?=
ifeq ($(LINT),1)
export YIO_CPPLINT=1
export YIO_CLANG_TIDY=1
endif

HELP_VAR +=~ R - Pass option to ctest -R
R ?=

###############################################################################

# Build dir name, matches .gitlab-ci
ifdef CC
ifneq ($(filter clang icc,$(CC)),)
_BCCNAME = $(CC)
endif
endif
_BCCNAME ?=
# OS IDentification, matches .gitlab-ci
OSID ?= $(shell test -r /etc/os-release && . /etc/os-release && echo $$ID)
ifneq ($(filter centos alpine,$(OSID)),)
_BOSNAME = $(OSID)
endif
_BOSNAME ?=
# PascalCase it
_BNAME ?= $(subst $(value 0) ,,\
		  $(foreach X,$(_BCCNAME) $(_BOSNAME) $(PRESET) $(CMAKE_BUILD_TYPE),$(shell V=$(X) ; V=$${V,,} ; echo $${V^})))
#
HELP_VAR +=~ B - Build directory location
B ?= _build/$(_BNAME)

# Default cmake and other flags
CMAKE = $(NICE) cmake
CTEST = $(NICE) ctest
CMAKEFLAGS += -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
CMAKEFLAGS += $(if $(value CMAKE_C_FLAGS),-DCMAKE_C_FLAGS="$(CMAKE_C_FLAGS)")
CMAKEFLAGS += --log-level=TRACE

SED_FIX_PATHS = sed -u 's@^[^ ]*/gen/@src/@; s@^\.\./\.\./test@test@'
GEN_TO_SRC = 2> >($(SED_FIX_PATHS) >&2) > >($(SED_FIX_PATHS))

BUILDFLAGS ?=

HELP_VAR +=~ V - set V=1 for verbose
V ?=
ifeq ($(V),1)
BUILDFLAGS += --verbose
CTESTFLAGS += -V
endif

ifdef ONE
CTESTFLAGS += -j 1 -V
R = ^$(ONE)$$
T = $(ONE)
endif

ifdef VALGRIND
CTESTFLAGS += -LE nomemcheck -D ExperimentalMemCheck
endif

TESTFLAGS ?= --output-on-failure -j $(NPROC)
CTESTFLAGS += $(if $(value R),-j 0 -R '$(R)')

###############################################################################

all: help

###############################################################################
# Generic configure+build+test targets

HELP +=~ configure - Configure the project
.PHONY: conf config configure
conf config configure $(B) $(B)/compile_commands.json:
	$(CMAKE) --preset=$(PRESET) -B$(B) -S. $(CMAKEFLAGS)

HELP +=~ .build_% - Generic target build
.build_%: unexport MAKEFLAGS
.build_%: conf
	$(CMAKE) --build $(B) $(BUILDFLAGS) --target $(if $(value R),$(shell cd $(B) && ninja -t targets | cut -d: -f1 | grep -v / | grep $(R) || echo all),$(if $(value T),$T,$*)) <&-

HELP +=~ build_gen - Only generate the files from m4 preprocessor
build_gen: .build_yio_gen

HELP +=~ build_yio - Build the yio target
build_yio: .build_yio

HELP +=~ build - Build the project
build: .build_all

HELP +=~ test - Run tests using ctest
test: build testonly

testonly:
	ulimit -c 0 ; cd $(B) && $(CTEST) $(TESTFLAGS) $(CTESTFLAGS)

TESTPROGRAM ?=
testprogram: export TESTPROGRAM := $(TESTPROGRAM)
testprogram: R = yio_testprogram
testprogram: .build_yio_testprogram
	$(B)/bin/yio_testprogram

###############################################################################

HELP +=~ valgrind - Run tests under valgrind
valgrind:
	du $(B)/Testing/Temporary/MemoryChecker.*.log | grep '^[^0]' | cut -f2- | xargs -r -d '\n' rm -v --
	ret=0 ; $(MAKE) test VALGRIND=1 || { ret=$$? ; tail -n +1 $(B)/Testing/Temporary/MemoryChecker.*.log ; } ; \
		! du $(B)/Testing/Temporary/MemoryChecker.*.log | grep '^[^0]' || exit "$$ret"

HELP +=~ clang
clang:
	CC=clang $(MAKE) test

###############################################################################

HELP +=~ cicd - Run all tests before commit to be sure it works
cicd: cicd.local cicd.gitlab
cicd.local:
	$(MAKE) CMAKE_BUILD_TYPE=RelWithDebInfo valgrind
	$(MAKE) CMAKE_BUILD_TYPE=RelWithDebInfo test_project
	$(MAKE) CMAKE_BUILD_TYPE=RelWithDebInfo sanitize
	$(MAKE) CMAKE_BUILD_TYPE=RelWithDebInfo clang
	$(MAKE) CMAKE_BUILD_TYPE=RelWithDebInfo arm
	$(MAKE) CMAKE_BUILD_TYPE=RelWithDebInfo gitlab_pages
cicd.gitlab:
	,gitlab run-gitlabci --runasroot --inplace alpine_test <&-
	,gitlab run-gitlabci --runasroot --inplace centos8_test <&-
icc:
	,gitlab run-gitlabci --runasroot --inplace icc_test
# Exotic Targets ##################################################

HELP +=~ sanitize
sanitize:
	$(MAKE) test PRESET=$@

HELP +=~ arm
arm:
	$(MAKE) test PRESET=$@

HELP +=~ arm2 - does not work
arm2:
	$(MAKE) test PRESET=$@

HELP +=~ sdcc - does not work
sdcc:
	$(MAKE) test PRESET=$@

HELP +=~ coverage
coverage:
	$(MAKE) test coverage_gen PRESET=$@

HELP +=~ coverage_gen
coverage_gen:
	$(MAKE) .coverage_gen PRESET=coverage
.coverage_gen:
	gcovr --print-summary --txt - --xml ./_build/cobertura-coverage.xml --json ./coverage.json --filter "$(B)/gen" --filter "src" -r . "$(B)"

namespace_stat: build_gen
	find gen/ -type f -name '*.[ch]' -exec cat {} + | \
		grep -v '@\|#line' | \
		tr -c '[:alnum:]_' '\n' | \
		grep -x '^[yY][[:alnum:]_]*_[[:alnum:]_]*' | \
		sort -u | \
		sed 's/_/ /' | \
		shuf | \
		awk '{ cnt[$$1]++; ex[$$1] = ex[$$1] (ex[$$1]?",":"") $$2; } END{ for (i in cnt) print cnt[i] "|" i "|" ex[i] }' | \
		sort -n | \
		column -t -s "|" -o ' ' | \
		if [ -t 1 ]; then cut -c-"$$(tput cols)"; else cat; fi

cproc:
	$(MAKE) test PRESET=$@

###############################################################################
# linting

HELP +=~ lint - run linters
HELP +=~ clang-tidy -
HELP +=~ cpplint -
lint:
	$(MAKE) clang-tidy cpplint
LINTOPTS = CMAKE_BUILD_TYPE=Debug PRESET=lint
clang-tidy cpplint cppcheck:
	$(MAKE) $(LINTOPTS) .build_yio_$@
.build_yio_cppcheck: export CMAKE_BUILD_PARALLEL_LEVEL=1

###############################################################################

test_alpine: USER  = $(shell id -u)
test_alpine: GROUP = $(shell id -g)
test_alpine:
	@mkdir -p _build/alpine/emptycontext
	docker build -t yio_alpine_builder -f alpine_builder.Dockerfile _build/alpine/emptycontext
	docker run -ti --rm -v $(CURDIR):/mnt -w /mnt -u ${USER}:${GROUP} \
		yio_alpine_builder make test B=_build/alpine

cmake-gui:
	cmake-gui -B_build/DefaultDebug -S.

###############################################################################
# Gitlab

HELP +=~ gitlab_pages - Generate gitlab pages
gitlab_pages: doxygen
	./scripts/create_public_index.sh

gitlab_metrics:
	./scripts/metrics.sh _build/Release | tee metrics.txt

###############################################################################
# Doxygen

HELP +=~ doxygen - Generates doxygen html documentation in pages/doxygen
.PHONY: doxygen
doxygen:
	$(MAKE) .doxygen.in CMAKE_BUILD_TYPE=RelWithDebInfo
.doxygen.in: build_gen
	mkdir -vp _build/doxygen/output _build/doxygen/input
	rsync --delete -a ./gen/ _build/doxygen/input/
	doxygen $(B)/Doxyfile
	mkdir -vp public/doxygen/
	rsync --delete -a _build/doxygen/output/html/ public/doxygen/
.PHONY: doxygen_open
doxygen_open:
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

buildclean:
	cmake --build $(B) --target clean

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


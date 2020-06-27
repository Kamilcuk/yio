MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables
.SUFFIXES:
export MAKEFLAGS

BUILDDIR = _build
DEPS = $(BUILDDIR)/deps
PREFIX = _build/public/

$(shell mkdir -p $(DEPS))

SHELL = bash

export KAMILCUKROWSKI_SECRET_GPG_KEY
export GPG_KEY = CC36098A43EBF7B5
export GPG_MASTER_KEY = 5B2030B2391B690AC869E1B59AB6D219060C0B5B
ifeq ($(shell hostname),leonidas)
export KAMILCUKROWSKI_SECRET_GPG_KEY=$(shell gpg --export-secret-subkeys $(GPG_KEY) | xxd -p -c 999999)
endif

ifeq ($(shell hash tput >/dev/null 2>&1 && echo true),true)
creset = $(shell echo -e '\e[0m')
crun = $(shell echo -e '\e[36m\e[1m')
cdone = $(shell echo -e '\e[32m\e[1m')
else
creset =
crun =
cdone =
endif

all: archlinux_x86_64
# all: archlinux_arm32v7
all: debian_x86_64
all: ubuntu_x86_64
all: alpine_x86_64

define begin
	@echo $(crun)::: RUN$(creset) $(notdir $1)
endef
define end
	@touch $1
	@echo $(cdone)::: DONE$(creset) $(notdir $1)
endef
define docker_run
	mkdir -p "$(BUILDDIR)/$1" "$(PREFIX)/$1"
	docker run -ti --rm \
		-e "KAMILCUKROWSKI_SECRET_GPG_KEY" \
		-e "GPG_KEY" \
		-e "GPG_MASTER_KEY" \
		-v $(abspath $(BUILDDIR)):/builddir \
		-e "BUILDDIR=/builddir/$1" \
		-v $(abspath $(PREFIX)):/prefix \
		-e "PREFIX=/prefix/$1" \
		-v $(abspath $(BUILDDIR)/arm-none-eabi-yio-package):/arm-none-eabi-yio-package \
		-e "ARM_NONE_EABI_YIO_PACKAGE=/arm-none-eabi-yio-package" \
		-v $(abspath ..):/work \
		-w /work
endef

define yio_docker_archlinux#(architecture, dockerimage)
yio_docker_archlinux_$1: $(DEPS)/yio_docker_archlinux_$1
$(DEPS)/yio_docker_archlinux_$1: archlinux/Dockerfile
$(DEPS)/yio_docker_archlinux_$1: archlinux/docker-setup.sh
	$$(call yio_docker_archlinux_in,$1,$2)
endef
define yio_docker_archlinux_in
	$(call begin,$@)
	docker build --build-arg FROM=$2 -t yio_docker_archlinux_$1 archlinux
	$(call end,$@)
endef
$(eval $(call yio_docker_archlinux,x86_64,archlinux))
$(eval $(call yio_docker_archlinux,arm32v7,agners/archlinuxarm-arm32v7))

define archlinux_def#(architecture)
archlinux_$1: $(DEPS)/archlinux_$1 ;
$(DEPS)/archlinux_$1: $(DEPS)/yio_docker_archlinux_$1
	$$(call archlinux_in,$1)
endef
define archlinux_in
	$(call begin,$@)
	$(call docker_run,archlinux/$1) \
	yio_docker_archlinux bash -xeuc ' \
		sed -i "s/if (( EUID == 0 )); then/if false; then/" /usr/bin/makepkg; \
		{ xxd -r -p | gpg --import ;} <<<"$$KAMILCUKROWSKI_SECRET_GPG_KEY"; \
		addnone=$$(if [[ "$1" == x86_64 ]]; then echo arm-none-eabi-yio; fi); \
		make -C pkg/archlinux PREFIX="$$PREFIX" "BUILDDIR=$$BUILDDIR" \
			yio $$addnone repo; \
		\
		pacman-key --keyserver hkp://keys.gnupg.net --recv-keys 5B2030B2391B690AC869E1B59AB6D219060C0B5B; \
		pacman-key --finger 5B2030B2391B690AC869E1B59AB6D219060C0B5B; \
		pacman-key --lsign-key 5B2030B2391B690AC869E1B59AB6D219060C0B5B; \
		printf "%s\n" "" "[yio]" "Server = file://$$PREFIX" | \
			tee /etc/pacman.conf; \
		pacman -Sy --noconfirm yio; \
		make test_project_no_install; \
	'
	$(call end,$@)
endef
$(eval $(call archlinux_def,x86_64))
$(eval $(call archlinux_def,arm32v7))
	
arm_none_eabi_yio_package: $(DEPS)/arm_none_eabi_yio_package
$(DEPS)/arm_none_eabi_yio_package: $(DEPS)/yio_docker_archlinux_x86_64
$(DEPS)/arm_none_eabi_yio_package: $(DEPS)/archlinux_x86_64
	$(call begin,$@)
	$(call docker_run,archlinux/x86_64) \
	yio_docker_archlinux_x86_64 bash -xeuc ' \
		rm -rf "$$ARM_NONE_EABI_YIO_PACKAGE"/*; \
		tar -C "$$ARM_NONE_EABI_YIO_PACKAGE" -axvf "$$PREFIX"/arm-none-eabi-yio*.pkg.tar.xz; \
		rm "$$ARM_NONE_EABI_YIO_PACKAGE"/.[^.]*; \
	'
	$(call end,$@)

define yio_docker_apt
yio_docker_$1: $(DEPS)/yio_docker_$1 ;
$(DEPS)/yio_docker_$1: apt/Dockerfile
	$$(call yio_docker_apt_in,$1)
endef
define yio_docker_apt_in
	$(call begin,$@)
	docker build --build-arg FROM=$1 -t $1 -f apt/Dockerfile apt
	$(call end,$@)
endef
$(eval $(call yio_docker_apt,debian))
$(eval $(call yio_docker_apt,ubuntu))

define apt_db
$1_x86_64: $(DEPS)/$1_x86_64
$(DEPS)/$1_x86_64: $(DEPS)/yio_docker_$1 
$(DEPS)/$1_x86_64: $(DEPS)/arm_none_eabi_yio_package
	$$(call apt_db_in,$1) 
endef
define apt_db_in
	$(call begin,$@)
	$(call docker_run,$1) \
	yio_docker_$1 bash -xeuc ' \
		{ xxd -r -p | gpg --import ;} <<<"$$KAMILCUKROWSKI_SECRET_GPG_KEY"; \
		make -C pkg/apt ARM_NONE_EABI_YIO_PACKAGE="$$ARM_NONE_EABI_YIO_PACKAGE" \
			PREFIX="$$PREFIX" "BUILDDIR=$$BUILDDIR" package package-arm-none-eabi-yio repo ; \
		\
		apt-key adv --keyserver hkp://keys.gnupg.net --recv-keys 5B2030B2391B690AC869E1B59AB6D219060C0B5B; \
		apt-key adv -a --export 5B2030B2391B690AC869E1B59AB6D219060C0B5B | apt-key add -; \
		echo "deb file://$$PREFIX/ $$(sed "/.*_CODENAME=/!d;s///;q" /etc/os-release) main" | \
			tee /etc/apt/sources.list.d/yio.list; \
		apt update -y; \
		apt install yio; \
		make test_project_no_install; \
	'
	$(call end,$@)
endef
$(eval $(call apt_db,debian))
$(eval $(call apt_db,ubuntu))

clean:
	sudo rm -rf $(BUILDDIR)

alpine_x86_64: $(DEPS)/alpine_x86_64
$(DEPS)/alpine_x86_64: $(wildcard alpine/*)
	$(call begin,$@)
	cd alpine && docker build --build-arg FROM=alpine -t yio_alpine_x86_64 .
	$(call docker_run,alpine/x86_64) \
	yio_alpine_x86_64 bash -c 'pkg/alpine/work.sh'
	$(call end,$@)

	











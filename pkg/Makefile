#!/bin/make -f
MAKEFLAGS += --no-builtin-rules --no-builtin-variables --warn-undefined-variables
.SUFFIXES:
export MAKEFLAGS
SHELL=bash

PREFIX = _build/output
OUTPUT := $(PREFIX)
BUILDDIR = _build
# Build directory shortcut
B := $(BUILDDIR)

export KAMIL_CUKROWSKI_SECRET_GPG_KEY
export GPG_KEY=CC36098A43EBF7B5
export GPG_MASTER_KEY=5B2030B2391B690AC869E1B59AB6D219060C0B5B
ifeq ($(shell hostname),leonidas)
export KAMIL_CUKROWSKI_SECRET_GPG_KEY=$(shell gpg --export-secret-subkeys CC36098A43EBF7B5 | base64 -w0)
endif

all: | \
	ctx \
	alpine_x86_64 \
	archlinux_x86_64 \
	ubuntu_x86_64 \
	debian_x86_64

# Cleanup target
clean:	
	rm -rf $(B)

# Context directory creation
ctxdir=_build/context
ctx=$(ctxdir)/srcdir.tar.gz
.PHONY: ctx
ctx: $(ctx)
$(ctx):
	mkdir -p $(B)/srcdir
	../scripts/copy_git_files_only.sh $(B)/srcdir
	mkdir -p $(ctxdir)
	tar -C _build -czf $(ctxdir)/srcdir.tar.gz srcdir || { rm -f $(ctxdir)/srcdir.tar.gz; false; }

# Abstract docker runner
define docker_build_in #(docker_image_name,output_directory,docker_build_arguments)
	docker build \
		--build-arg KAMIL_CUKROWSKI_SECRET_GPG_KEY \
		$3 \
		-t $1 \
		.
	mkdir -p $2
	docker run --rm $1 tar -C /output -czf - . | tar -C $2 -xvz
endef
# Note: the first argument _has to_ be blabal/distribution/architecture
define docker_build #(output_directory_like_/distribution/architecture,docker_build_arguments)
	$(eval _tmp_docker_build := $(dir $1))
	$(call docker_build_in,yio_$(notdir $(_tmp_docker_build:/=))_$(notdir $1),$1,$2)
endef

alpine_x86_64: $(OUTPUT)/alpine/x86_64
$(OUTPUT)/alpine/x86_64:
	$(call docker_build,$@, \
		--file alpine.Dockerfile \
		--build-arg FROM=alpine \
	)

archlinux_x86_64: $(OUTPUT)/archlinux/x86_64
$(OUTPUT)/archlinux/x86_64:
	$(call docker_build,$@, \
		--file archlinux.Dockerfile \
		--build-arg FROM=archlinux \
	)

# arm-none-eabi-yio.tar.gz is the same for all distributions...
# So I'll extract it from archlinux package and then inject
# into ubuntu and debian, who cares
nonetar=_build/context/arm-none-eabi-yio.tar.zst
$(nonetar): $(OUTPUT)/archlinux/x86_64
	mkdir -p _build/arm-none-eabi-yio
	cp $(OUTPUT)/archlinux/x86_64/arm-none-eabi-yio*.pkg.tar.zst _build/arm-none-eabi-yio
	cd _build/arm-none-eabi-yio && tar -xaf arm-none-eabi-yio*.pkg.tar.zst
	cd _build/arm-none-eabi-yio && rm -f arm-none-eabi-yio*.pkg.tar.zst *.pkg.tar
	cd _build/arm-none-eabi-yio && rm .[^.]*
	cd _build && tar -czf context/arm-none-eabi-yio.tar.gz arm-none-eabi-yio

ubuntu_x86_64: $(OUTPUT)/ubuntu
$(OUTPUT)/ubuntu: $(nonetar)
	$(call docker_build_in,yio_ubuntu_x86_64,$@, \
		--file apt.Dockerfile \
		--build-arg FROM=ubuntu \
	)

debian_x86_64: $(OUTPUT)/debian
$(OUTPUT)/debian: $(nonetar)
	$(call docker_build_in,yio_debian_x86_64,$@, \
		--file apt.Dockerfile \
		--build-arg FROM=debian \
	)




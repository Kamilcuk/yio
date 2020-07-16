MAKEFLAGS += -r
export MAKEFLAGS

BUILDDIR ?= _build
PREFIX ?= _build/public
VERSION = $(shell git describe --tags | sed 's/[^0-9]//')

ifneq (,$(wildcard /etc/debian_version))
system = DEBIAN
else
system = ubuntu
endif

CODENAME = $(shell sed "/.*_CODENAME=/!d;s///;q" /etc/os-release)

all: package repo

package: package-yio
package-yio: $(BUILDDIR)/package-yio
$(BUILDDIR)/package-yio:
	cmake \
		-S ../.. \
		-B $(BUILDDIR)/yio \
		-D CMAKE_BUILD_TYPE=Release \
		-Wno-dev
	cd $(BUILDDIR)/yio && cpack -G DEB
	touch $@

package-arm-none-eabi-yio: $(BUILDDIR)/package-arm-none-eabi-yio
$(BUILDDIR)/package-arm-none-eabi-yio: $(ARM_NONE_EABI_YIO_PACKAGE) arm-none-eabi-yio/control
$(BUILDDIR)/package-arm-none-eabi-yio: D = arm-none-eabi-yio_$(VERSION)
$(BUILDDIR)/package-arm-none-eabi-yio:
	rm -rf  $(BUILDDIR)/$(D)/*
	mkdir -p $(BUILDDIR)/$(D)
	test -e '$(ARM_NONE_EABI_YIO_PACKAGE)'
	cp -a $(ARM_NONE_EABI_YIO_PACKAGE)/* $(BUILDDIR)/arm-none-eabi-yio
	mkdir -p $(BUILDDIR)/$(D)/$(system)
	cp -a arm-none-eabi-yio/* $(BUILDDIR)/$(D)/$(system)
	./git2debchangelog.sh > $(BUILDDIR)/$(D)/$(system)/changelog
	sed -i 's/@VERSION@/$(VERSION)/' $(BUILDDIR)/$(D)/$(system)/control
	cd $(BUILDDIR) && dpkg-deb --build $(D)
	# touch $@

repo: $(PREFIX)/db ;
.PHONY: $(PREFIX)/db
$(PREFIX)/db: $(wildcard conf/*) Makefile
	rm -rf $(PREFIX)/*
	mkdir -p $(PREFIX)
	cp -a conf/ $(PREFIX)
	sed -i 's/%CODENAME%/$(CODENAME)/' $(PREFIX)/conf/distributions
	find $(BUILDDIR) -maxdepth 3 -type f -name '*.deb' | \
	xargs -d '\n' -t reprepro --basedir $(PREFIX) --verbose includedeb $(CODENAME)

clean:
	rm -rf _build


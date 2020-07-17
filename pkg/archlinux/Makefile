MAKEFLAGS += -r
export MAKEFLAGS

# Build dir
BUILDDIR = _build
PREFIX = _build/public

B = $(BUILDDIR)
P = $(PREFIX)

GPG_KEY ?= CC36098A43EBF7B5
# 5B2030B2391B690AC869E1B59AB6D219060C0B5B
SIGN = --sign --key $(GPG_KEY)
export YIO_SRCDIR = $(abspath ../..)

all: yio arm-none-eabi-yio repo

yio: $(B)/packaged-yio ;
arm-none-eabi-yio: $(B)/packaged-arm-none-eabi-yio ;
$(B)/packaged-%: PKGBUILD-%
	mkdir -p $(B)/build-$*
	cp PKGBUILD-$* $(B)/build-$*/PKGBUILD
	cd $(B)/build-$* && makepkg $(SIGN) -f --noconfirm --needed --noprogressbar --syncdeps
	touch $@

repo: $(P)/yio.db.tar.xz
$(P)/yio.db.tar.xz: yio $(wildcard $(B)/*/*.pkg.tar.xz)
	mkdir -p $(P)
	find $(B)/build-* -maxdepth 1 -mindepth 1 -type f -name '*.pkg.tar.*' -exec cp -v {} $(P) \;
	cd $(P) && find . -maxdepth 1 -mindepth 1 -type f -name '*.pkg.tar.*' '!' -name '*.pkg.tar.*.sig' -exec repo-add $(SIGN) yio.db.tar.xz {} \;
	find $(P) -type f | sort

clean:
	rm -rf $(B)


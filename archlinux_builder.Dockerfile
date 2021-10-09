FROM archlinux
RUN pacman --noconfirm -Sy \
		git \
		make cmake m4 gcc ninja libunistring \
 		clang graphviz doxygen jq \
		arm-none-eabi-gcc arm-none-eabi-newlib \
		valgrind rsync \
		qemu-headless-arch-extra \
		sdcc \
		distcc


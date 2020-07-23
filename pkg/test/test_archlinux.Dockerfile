FROM archlinux
RUN set -x && \
		pacman-key --init && \
		pacman -Syu --noconfirm sudo git cmake m4 make gcc


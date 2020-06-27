#!/bin/bash
set -xeuo pipefail

if [ ! -e /.dockerenv ]; then
	echo "error: this script is meant to be in docker" >&2
	exit 2
fi

echo 'Installing packages'
pacman-key --init
pacman -Sy --noconfirm --needed archlinux-keyring
pacman -Suy --noconfirm --needed make m4 cmake gcc git ca-certificates curl libunistring ninja sudo base-devel vim base-devel
echo 'Cleanup'
printf 'y\ny\n' | pacman -Scc ||:

echo "setup builder account"
if ! grep -q builder /etc/passwd; then

	useradd builder -u 1000 --system --shell /sbin/nologin --home-dir /var/cache/build --create-home
	passwd -d builder
	chmod +w /etc/sudoers
	echo "builder ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
	echo "root ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
	chmod -w /etc/sudoers

	sudo -u builder bash <<'EOF'
set -x

echo "Disable key checking in ssh config"
mkdir -p ~/.ssh 
echo -e "Host *\n\tStrictHostKeyChecking no\n\n" > ~/.ssh/config

echo "Use pacman pubring in builder gpg"
mkdir -p ~/.gnupg
chmod 700 ~/.gnupg
echo "keyring /etc/pacman.d/gnupg/pubring.gpg" >> ~/.gnupg/gpg.conf

EOF

else
	echo "builder user already exists"
fi

sed -i -e 's/^MAKEFLAGS=*/MAKEFLAGS=-j$(nproc)/' /etc/makepkg.conf


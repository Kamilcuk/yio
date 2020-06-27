#!/bin/bash
set -xeuo pipefail

: ${BUILDDIR:=_build}
: ${PREFIX:=_build/public}

cd "$(dirname "$0")"
export YIO_SRCDIR=$(readlink -f ../..)

mkdir -p "$BUILDDIR"
mkdir -p "$PREFIX"

echo 'PACKAGER="Kamil Cukrowski <kamilcukrowski@gmail.com>"' >> /etc/abuild.conf
echo 'MAINTAINER="$PACKAGER"' >> /etc/abuild.conf

adduser builder -D
addgroup builder abuild
chmod +w /etc/sudoers
echo 'builder ALL=(ALL) ALL' >> /tmp/sudoers
chmod -w /etc/sudoers

sudo -u builder git config --global user.name "Kamil Cukrowski"
sudo -u builder git config --global user.email "kamilcukrowski@gmail.com"

mkdir -p /var/cache/distfiles
chmod a+w /var/cache/distfiles
chgrp abuild /var/cache/distfiles
chmod g+w /var/cache/distfiles

key=/home/builder/.abuild/kamilcukrowski@gmail.com.rsa
pub=/etc/apk/keys/kamilcukrowski@gmail.com.rsa.pub
mkdir -p "$(dirname "$key")"
{ xxd -r -p | sudo -u builder gpg --import ;} <<<"$KAMILCUKROWSKI_SECRET_GPG_KEY"; 
sudo -u builder gpg --export-secret-keys CC36098A43EBF7B5 |
	openpgp2ssh CC36098A43EBF7B5 > "$key"
chmod 400 "$key"
ssh-keygen -y -f "$key" > "$pub"
chown builder "$pub"
chown builder -R "$(dirname "$key")"

echo 'PACKAGER_PRIVKEY=/home/builder/.abuild/kamilcukrowski@gmail.com.rsa' >> /etc/abuild.conf

apkbuild-lint APKBUILD
sudo -u builder env YIO_SRCDIR="$YIO_SRCDIR" abuild -r

sudo -u builder abuild-sign -k /home/builder/.abuild/kamilcukrowski@gmail.com.rsa \
	/home/builder/packages/*/*/APKINDEX.tar.gz 
cp -va /home/builder/packages/pkg/*/* "$PREFIX"
	



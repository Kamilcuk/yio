ARG FROM
FROM $FROM

RUN apk add --no-cache \
		alpine-sdk sudo gnupg bash monkeysphere libunistring-dev git atools cmake m4

SHELL ["/bin/bash", "-c"]
ARG KAMIL_CUKROWSKI_SECRET_GPG_KEY
RUN set -x &&\
	\
	mkdir /output &&\
	\
	echo 'PACKAGER="Kamil Cukrowski <kamilcukrowski@gmail.com>"' >> /etc/abuild.conf &&\
	echo 'MAINTAINER="$PACKAGER"' >> /etc/abuild.conf &&\
	\
	adduser builder -D &&\
	addgroup builder abuild &&\
	chmod +w /etc/sudoers &&\
	echo 'builder ALL=(ALL) ALL' >> /tmp/sudoers &&\
	chmod -w /etc/sudoers &&\
	\
	sudo -u builder git config --global user.name "Kamil Cukrowski" &&\
	sudo -u builder git config --global user.email "kamilcukrowski@gmail.com" &&\
	\
	mkdir -p /var/cache/distfiles &&\
	chmod a+w /var/cache/distfiles &&\
	chgrp abuild /var/cache/distfiles &&\
	chmod g+w /var/cache/distfiles &&\
	\
	key=/home/builder/.abuild/kamilcukrowski@gmail.com.rsa &&\
	pub=/etc/apk/keys/kamilcukrowski@gmail.com.rsa.pub &&\
	mkdir -p "$(dirname "$key")" &&\
	{ base64 -d | sudo -u builder gpg --import ;} <<<"$KAMIL_CUKROWSKI_SECRET_GPG_KEY" &&\
	sudo -u builder gpg --export-secret-keys CC36098A43EBF7B5 | openpgp2ssh CC36098A43EBF7B5 > "$key" &&\
	chmod 400 "$key" &&\
	ssh-keygen -y -f "$key" > "$pub" &&\
	chown builder "$pub" &&\
	chown builder -R "$(dirname "$key")" &&\
	\
	echo 'PACKAGER_PRIVKEY=/home/builder/.abuild/kamilcukrowski@gmail.com.rsa' >> /etc/abuild.conf
ARG KAMIL_CUKROWSKI_SECRET_GPG_KEY=

WORKDIR /home/builder
COPY _build/context/srcdir.tar.gz .
RUN set -x &&\
	\
	tar -xzf srcdir.tar.gz &&\
	cd srcdir/pkg/alpine &&\
	apkbuild-lint APKBUILD &&\
	sudo -u builder env YIO_SRCDIR="/home/builder/srcdir" abuild -r &&\
	cd /home/builder &&\
	rm -r srcdir &&\
	sudo -u builder abuild-sign -k /home/builder/.abuild/kamilcukrowski@gmail.com.rsa \
		/home/builder/packages/*/*/APKINDEX.tar.gz &&\
	cp -va /home/builder/packages/pkg/*/* /output &&\
	openssl rsa -in /etc/apk/keys/kamilcukrowski@gmail.com.rsa.pub -pubout -out /output/kamilcukrowski@gmail.com.rsa.pub


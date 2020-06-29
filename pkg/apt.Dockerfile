ARG FROM
FROM ${FROM}
RUN \
	DEBIAN_FRONTEND=noninteractive apt update -y && \
	DEBIAN_FRONTEND=noninteractive apt install -y build-essential vim gpg m4 cmake \
		gcc git ca-certificates curl libunistring-dev ninja-build reprepro \
		git-extras devscripts \
	&& \
	rm -rf /var/lib/apt/lists/*

SHELL ["/bin/bash", "-c"]
ARG KAMIL_CUKROWSKI_SECRET_GPG_KEY
RUN { base64 -d | gpg --import ;} <<<"$KAMIL_CUKROWSKI_SECRET_GPG_KEY"
ARG KAMIL_CUKROWSKI_SECRET_GPG_KEY=

COPY _build/context/arm-none-eabi-yio.tar.gz /
COPY _build/context/srcdir.tar.gz /
RUN set -x &&\
    \
	tar -xzf arm-none-eabi-yio.tar.gz && \
    tar -xzf srcdir.tar.gz && cd srcdir &&\
	make -C pkg/apt ARM_NONE_EABI_YIO_PACKAGE="/arm-none-eabi-yio" \
		PREFIX="/output" package package-arm-none-eabi-yio repo  &&\
	\
	apt-key adv --keyserver hkp://keys.gnupg.net --recv-keys 5B2030B2391B690AC869E1B59AB6D219060C0B5B &&\
	apt-key adv -a --export 5B2030B2391B690AC869E1B59AB6D219060C0B5B | apt-key add - &&\
	echo "deb file:///output $(sed "/.*_CODENAME=/!d;s///;q" /etc/os-release) main" | \
		tee /etc/apt/sources.list.d/yio.list &&\
	apt update -y &&\
	apt install yio &&\
	make test_project_no_install



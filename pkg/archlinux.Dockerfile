ARG FROM
FROM $FROM
COPY archlinux/docker-setup.sh /
RUN /docker-setup.sh && rm docker-setup.sh
SHELL ["/bin/bash", "-c"]
ARG KAMIL_CUKROWSKI_SECRET_GPG_KEY
RUN set -x &&\
		sed -i "s/if (( EUID == 0 )); then/if false; then/" /usr/bin/makepkg &&\
		{ base64 -d | gpg --import ;} <<<"$KAMIL_CUKROWSKI_SECRET_GPG_KEY"
ARG KAMIL_CUKROWSKI_SECRET_GPG_KEY=

COPY _build/context/srcdir.tar.gz .
RUN set -x &&\
		tar -xzf srcdir.tar.gz &&\
		cd srcdir &&\
		addnone=$(if [[ "$(uname -m)" == x86_64 ]]; then echo arm-none-eabi-yio; fi) &&\
		make -C pkg/archlinux PREFIX="/output" yio $addnone repo &&\
		pacman-key --keyserver hkp://keys.gnupg.net --recv-keys 5B2030B2391B690AC869E1B59AB6D219060C0B5B &&\
		pacman-key --finger 5B2030B2391B690AC869E1B59AB6D219060C0B5B &&\
		pacman-key --lsign-key 5B2030B2391B690AC869E1B59AB6D219060C0B5B &&\
		printf "%s\n" "" "[yio]" "Server = file:///output" | tee /etc/pacman.conf &&\
		pacman -Sy --noconfirm yio &&\
		make test_project_no_install


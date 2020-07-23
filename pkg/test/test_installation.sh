#!/bin/bash
set -euo pipefail

testit="
git clone https://gitlab.com/Kamcuk/yio.git /yio
cd /yio
make test_project_no_install
"

debian() {
	docker build --build-arg FROM=debian -f test_ubuntu_debian.Dockerfile -t yio_test_debian .
	docker run -i --rm yio_test_debian bash -xeu <<EOF
sudo apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-keys 9AB6D219060C0B5B
sudo apt-key adv -a --export 9AB6D219060C0B5B | sudo apt-key add -
echo 'deb https://kamcuk.gitlab.io/yio/debian buster main' | sudo tee -a /etc/apt/sources.list.d/yio.list
sudo apt-get update
sudo apt-get install -y yio
$testit
EOF
}

ubuntu() {
	docker build --build-arg FROM=ubuntu -f test_ubuntu_debian.Dockerfile -t yio_test_ubuntu .
	docker run -i --rm yio_test_ubuntu bash -xeu <<EOF
sudo apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-keys 9AB6D219060C0B5B
sudo apt-key adv -a --export 9AB6D219060C0B5B | sudo apt-key add -
echo 'deb https://kamcuk.gitlab.io/yio/ubuntu focal main' | sudo tee -a /etc/apt/sources.list.d/yio.list
sudo apt-get update
sudo apt-get install -y yio
$testit
EOF
}

archlinux() {
	docker build -f test_archlinux.Dockerfile -t yio_test_archlinux .
	docker run -i --rm yio_test_archlinux bash -xeu <<EOF
sudo pacman-key --init
sudo pacman-key --keyserver hkp://pool.sks-keyservers.net --recv-keys 9AB6D219060C0B5B
sudo pacman-key --lsign-key 9AB6D219060C0B5B
sudo cp /etc/pacman.conf /etc/pacman.conf.backup
printf "%s\n" '' '[yio]' 'Server = https://kamcuk.gitlab.io/yio/archlinux/\$arch/' '' | sudo tee -a /etc/pacman.conf
sudo pacman -Sy --noconfirm yio arm-none-eabi-yio
$testit
EOF
}

alpine() {
	docker build -f test_alpine.Dockerfile -t yio_test_alpine .
	docker run -i --rm yio_test_alpine sh -xeu <<EOF
wget 'https://kamcuk.gitlab.io/yio/alpine/x86_64/kamilcukrowski@gmail.com.rsa.pub' -O /etc/apk/keys/kamilcukrowski@gmail.com.rsa.pub
echo 'https://kamcuk.gitlab.io/yio/alpine/' >> /etc/apk/repositories
apk add --allow-untrusted yio
$testit
EOF
}

case "$1" in
debian|ubuntu|archlinux|alpine) "$1"; ;;
esac


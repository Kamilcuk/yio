#!/bin/bash
set -xeuo pipefail

debian() {
docker run -i --rm debian bash <<'EOF'
set -xeuo pipefail
apt-get update
apt-get install -y sudo gnupg ca-certificates
sudo apt-key adv --keyserver hkp://keys.gnupg.net --recv-keys 5B2030B2391B690AC869E1B59AB6D219060C0B5B
sudo apt-key adv -a --export 5B2030B2391B690AC869E1B59AB6D219060C0B5B | sudo apt-key add -
echo 'deb https://kamcuk.gitlab.io/yio/debian buster main' | sudo tee -a /etc/apt/sources.list.d/yio.list
sudo apt-get update
sudo apt-get install -y yio
EOF
}

ubuntu() {
docker run -i --rm ubuntu bash <<'EOF'
set -xeuo pipefail
apt-get update
apt-get install -y sudo gnupg ca-certificates
sudo apt-key adv --keyserver hkp://keys.gnupg.net --recv-keys 5B2030B2391B690AC869E1B59AB6D219060C0B5B
sudo apt-key adv -a --export 5B2030B2391B690AC869E1B59AB6D219060C0B5B | sudo apt-key add -
echo 'deb https://kamcuk.gitlab.io/yio/ubuntu focal main' | sudo tee -a /etc/apt/sources.list.d/yio.list
sudo apt-get update
sudo apt-get install -y yio
EOF
}

archlinux() {
docker run -i --rm archlinux bash <<'EOF'
set -xeuo pipefail
pacman -Syu --noconfirm sudo
pacman-key --init
sudo pacman-key --keyserver hkp://keys.gnupg.net --recv-keys 5B2030B2391B690AC869E1B59AB6D219060C0B5B
sudo pacman-key --finger 5B2030B2391B690AC869E1B59AB6D219060C0B5B
sudo pacman-key --lsign-key 5B2030B2391B690AC869E1B59AB6D219060C0B5B
sudo cp /etc/pacman.conf /etc/pacman.con.backup
printf "%s\n" '' '[yio]' 'Server = https://kamcuk.gitlab.io/yio/archlinux/$arch/' '' | sudo tee -a /etc/pacman.conf
sudo pacman -Sy --noconfirm yio
EOF
}

alpine() {
docker run -i --rm alpine sh <<'EOF'
set -xeu
uget 'https://kamcuk.gitlab.io/yio/alpine/kamilcukrowski@gmail.com.rsa.pub' -O /etc/apk/keys/kamilcukrowski@gmail.com.rsa.pub
echo 'https://kamcuk.gitlab.io/yio/alpine/' >> /etc/apk/repositories
apk add yio
EOF
}

if (($#)); then
	"$1"
else
	debian
	ubuntu
	archlinux
	alpine
fi



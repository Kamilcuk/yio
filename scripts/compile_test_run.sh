#!/bin/bash
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"/..
gen="./gen"
libs=$(
	find _build/Debug/src -maxdepth 1 -name '*.a' -type f -printf "%f\n" |
	sed 's/lib/-l/;s/\.a//'
)

tmp=$(mktemp)
trap 'rm "$tmp"' EXIT
{
	printf '#include <%s.h>\n' yio ywio yuio
	if (($#)); then
		printf "%s\n" "$@"
	else
		cat
	fi
} | {
	set -x
	gcc -L_build/Debug/src -Wall -Wl,--whole-archive $libs -lunistring -lm -Wl,--no-whole-archive -I"$gen" -o "$tmp" -xc -
} && "$tmp"


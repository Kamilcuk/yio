#!/bin/bash

cd "$(dirname "$BASH_SOURCE")"/..
tmp=$(mktemp)
trap 'rm "$tmp"' EXIT
{
	printf '#include <%s.h>\n' yio ywio yuio
	cat
} | (
run=(
	gcc -L_build/Debug/src -Wl,--whole-archive $(
		find _build/Debug/src -maxdepth 1 -name '*.a' -type f -printf "%f\n" |
		sed 's/lib/-l/;s/\.a//'
	) -lunistring -lm -Wl,--no-whole-archive -I$(readlink -f gen) -o "$tmp" -xc -
)
set -x
"${run[@]}" && "$tmp"
)


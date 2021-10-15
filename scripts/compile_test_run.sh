#!/bin/bash
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"/..
gen="./gen"
#libs=$(
	#find _build/Debug/lib -maxdepth 1 -name '*.a' -type f -printf "%f\n" |
	#sed 's/lib/-l/;s/\.a//'
#)

opts=()
while (($#)); do
	case "$1" in
	-*) opts+=("$1"); ;;
	*) break;
	esac
	shift
done

tmp=$(mktemp)
trap 'rm "$tmp"' EXIT
src=$(
	printf '#include <%s.h>\n' yio ywio yuio
	if (($#)); then
		printf "%s\n" "$@"
	else
		cat
	fi
)
cmd=(
	gcc -I"$gen" -L_build/Debug/lib -Wall -o "$tmp" -xc - "${opts[@]}" -lyio -lunistring -lm
)

echo "$src" >&2
echo "+ ${cmd[*]}" >&2
<<<"$src" "${cmd[@]}" && "$tmp"


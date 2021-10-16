#!/bin/bash
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"/..
lib=$(ls -td _build/*/lib | head -n1)
lib=$(dirname "$lib")

opts=(-fdata-sections -ffunction-sections -Wl,--gc-sections)
while (($#)); do
	case "$1" in
	-use=*) lib=${1#*=}; ;;
	-custom) opts+=(-DYIO_PRINT_FLOATS_WITH=YIO_PRINT_FLOATS_WITH_CUSTOM); ;;
	-ryu) opts+=(-DYIO_PRINT_FLOATS_WITH=YIO_PRINT_FLOATS_WITH_RYU); ;;
	-*) opts+=("$1"); ;;
	*) break;
	esac
	shift
done

gen="$lib"/gen
lib="$lib"/lib

tmp=$(mktemp)
trap 'rm "$tmp"' EXIT
src=$(
	printf '#include <%s.h>\n' yio ywio yuio float
	if (($#)); then
		printf "%s\n" "$@"
	else
		cat
	fi
)
cmd=(
	${CC:-gcc} -I"$gen" -L"$lib" -Wall -o "$tmp" -xc - "${opts[@]}" -lyio -lunistring -lm
)

echo "$src" >&2
echo "+ ${cmd[*]}" >&2
<<<"$src" "${cmd[@]}" && "$tmp"


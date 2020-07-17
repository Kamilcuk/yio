#!/bin/bash
set -euo pipefail

verboseflag=
while (($#)); do
	case "$1" in
	-v) verboseflag=-v; ;;
	*) break;
	esac
	shift
done

name=$(basename "$0")
if [[ -t 1 ]] && (($# <= 1)); then
	cat <<EOF
Usage: 
	echo file1 file2 dir1 | $name destinationdir
	$name file1 file2 dir1 destinationdir

Copies files to destination.
Removes all unrelated files.

EOF
	exit 1
fi

if (($# == 1)); then
	mkdir -p "$1"
	rsync --delete --files-from=/dev/stdin $verboseflag -a . "$1"
else
	dir="${@: -1}"
	mkdir -p "$dir"
	rsync --delete $verboseflag -a "$@"
fi



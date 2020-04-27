#!/bin/sh

usage() {
	cat <<EOF
Usage: $(basename "$0") \\
	<m4_executable> <output_file> \\
	<options_separated_by_semicolon> \\
	<sources_and_additional_m4_options>

A helper script to be run from cmake as m4 preprocessor.

Written by Kamil Cukrowski.
EOF
}

if [ "$#" -lt 4 ]; then
	usage
	exit 2
fi

m4="$1"
f="$2"
opts="$3"
shift 3

if ! err=$( { IFS=';'; "$m4" $opts "$@" > "$f" ;} 2>&1 ); then
	# remove the initial executable name from the error message
	# for eclipse for fast navigation
	printf "%s\n" "$err" | sed >&2 "s;^$m4:;;"
	# if you want to see additional ": m4: "
	# s;: ;&m4&;" >&2
	exit 1
fi
chmod ugo-w "$f" 2>/dev/null >&1 ||:

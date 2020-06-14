#!/bin/sh

name=$(basename "$0")

usage() {
	cat <<EOF
Usage: $name \\
	<m4_executable> <output_file> \\
	<options_separated_by_semicolon> \\
	<sources_and_additional_m4_options>

A helper script to be run from cmake as m4 preprocessor.

Written by Kamil Cukrowski.
EOF
}

fatal() {
	echo "$name: Error:" "$*" >&2
	exit 2
}

# Main #######################

if [ "$#" -lt 4 ]; then
	usage
	exit 2
fi

m4="$1"
f="$2"
opts="$3"
shift 3

# Check m4 executable
if [ ! -x "$m4" ] && ! hash "$m4" >/dev/null 2>&1; then
	fatal "No such command: $m4"
fi

# Create destination directory
fdir=$(dirname "$f")
if ! mkdir -p "$fdir"; then
	fatal "Could not create directory: $fdir"
fi

# If the destination file exists and is not writable
if [ -e "$f" -a ! -w "$f" ]; then
	# make it writable
	if ! chmod +w "$f" >/dev/null 1>&2; then
		fatal "Could not chmod +w: $f"
	fi
fi

if ! err=$( { IFS=';'; "$m4" $opts "$@" > "$f" ;} 2>&1 ); then
	# remove the initial executable name from the error message
	# for eclipse for fast navigation
	printf "%s\n" "$err" | sed >&2 "s;^$m4:;;"
	# if you want to see additional ": m4: "
	# s;: ;&m4&;" >&2
	exit 1
fi

# Make destination file not writable
# so we don't edit it from IDE
chmod -w "$f" >/dev/null >&1 ||:


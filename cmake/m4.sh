#!/bin/sh
set -eu
export SHELLOPTS

name=$(basename "$0")

usage() {
	cat <<EOF
Usage: $name [Options] -- <m4_executable> <output_file> <m4_options> 

Options:
  -MF <file>    Generate dependency file
  -MT <target>  Target name for dependency file 

A helper script to be run from cmake as m4 preprocessor.
The -MF -MT options are the same as in gcc and can be used
to generate dependency files.
Dependency generation uses output from m4 and depends on '-dp'
m4 option support and format.

Written by Kamil Cukrowski.
EOF
}

fatal() {
	echo "$name: Error:" "$*" >&2
	exit 2
}

#####################################################################
# Main

# Parse arguments
target=
depfile=
while [ "$#" -ne 0 ]; do
	case "$1" in
	-MF) depfile="$2"; shift; ;;
	-MT) deptarget="$2"; shift; ;;
	--) shift; break; ;;
	*) break; ;;
	esac
	shift
done

if [ "$#" -lt 3 ]; then
	usage
	exit 2
fi

m4="$1"
outputf="$2"
shift 2

# Check m4 executable
if [ ! -x "$m4" ] || ! hash "$m4" >/dev/null 2>&1; then
	fatal "No such command: $m4"
fi

# Create destination directory
fdir=$(dirname "$outputf")
if ! mkdir -p "$fdir"; then
	fatal "Could not create directory: $fdir"
fi

# If the destination file exists and is not writable
if [ -e "$outputf" ] && [ ! -w "$outputf" ]; then
	# make it writable
	if ! chmod +w "$outputf"; then
		fatal "Could not chmod +w: $outputf"
	fi
fi

# Matches m4debug dependency messages.
deprgx="^m4debug: path search for \`.*' found \`\\(.*\\)'\$"

if ! err=$( { "$m4" -dp "$@" > "$outputf" ;} 2>&1 ); then
	# Remove the initial executable name from the error message
	# for eclipse for fast navigation
	printf "%s\n" "$err" | sed "/$deprgx/"'d; s/^m4://' >&2
	exit 1
fi

if [ -n "$depfile" ]; then
	{
		echo "${deptarget:-$outputf}:"
		printf "%s\n" "$err" | sed "/$deprgx/"'!d; s//\1/'		
	} | paste -sd ' ' > "$depfile"
fi

# Make destination file not writable
# so we don't edit it from IDE
chmod -w "$outputf" >/dev/null >&1 ||:






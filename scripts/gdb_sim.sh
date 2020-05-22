#!/bin/bash
set -euo pipefail

usage() {
	cat <<EOF
Usage:
  gdb_sim.sh [OPTIONS] executable [arguments...]

Run executable under gdb with 'target sim'.

Options:
  -g <gdb>  Path to the gdb executable
  -v        Be verbose. Can be used multiple times
  -i        Interactive connect to gdb after loading the binary

Written by Kamil Cukrowski 2019.
EOF
	if (($#)); then
		echo "$@" >&2
		exit 1
	fi
}

fatal() {
	echo "$0:" "$*" >&2
	exit 2
}

# main() ###################################

args=$(getopt -n "gdb_sim.sh" -o vg:i -- "$@")
eval set -- "$args"
gdb="gdb"
verbose=false
interactive=false
while (($#)); do
	case "$1" in
		-g) shift; gdb="$1"; ;;
		-v) if $verbose; then set -x; fi; verbose=true; ;;
		-i) interactive=true; ;;
		--) shift; break; ;;
		*) echo "Internal error" >&2; exit 1 ;;
	esac
	shift
done

if (($# == 0)); then
	usage "Error: executable argument expected"
fi

executable=$1
shift
arguments=("$@")

if [[ ! -e "$executable" ]]; then
	fatal "$executable: No such file or directory"
fi
if [[ ! -x "$executable" ]]; then
	fatal "$executable: Permission denied"
fi
if ! hash "$gdb" >/dev/null; then
	fatal "$gdb: command not found"
fi

set -o monitor
set -m

tmp=$(mktemp -d)
trap_exit() {
	if [[ -n "$(jobs -pr)" ]]; then
		kill $(jobs -pr) ||:
	fi;
	if [[ -e "$tmp" ]]; then
		rm -r "$tmp" ||:
	fi
}
trap 'trap_exit' EXIT

in="$tmp/in" out="$tmp/out"
mkfifo "$in" "$out"

waitforline() {
	local line
	line=$1
	while IFS= read -r -u 10 -t 1 i && [[ -n "$i" ]]; do
		if [[ "$i" =~ $line ]]; then
			break
		fi
	done
}

waitforlineoutput() {
	local rgx=$1
	while IFS= read -r -u 10 i && [[ -n "$i" ]]; do
		if [[ "$i" =~ $rgx ]]; then
			break
		fi
		printf "%s\n" "$i"
	done
}

gdb=("$gdb" -quiet --args "$executable" "${arguments[@]}")
child() {
	if "$verbose"; then
		trap_exit() { echo > gdb child dying >&3; }
		trap 'trap_exit' EXIT

		stdbuf -oL tee >(sed -u 's/^/< /' >&2) |
		stdbuf -oL "${gdb[@]}" 2>&1 |
		stdbuf -oL tee >(sed -u 's/^/> /' >&2)
		echo >&2
	else
		stdbuf -oL "${gdb[@]}" 2>&1
	fi
}
child >"$in" <"$out" 3>&2 &
childpid=$!

exec 10< "$in"
exec 11> "$out"

uuid=68ba305a-5761-4afb-bff8-079ea0619b4f

cat >&11 <<EOF
set confirm off
target sim
load
break exit
break _exit
break _Exit
break raise
break signal
break abort
echo \n$uuid\n
EOF

waitforline "^$uuid\$"

if "${interactive:-false}"; then
    cat <&10 &
    cat >&11
    exit
fi

echo 'run' >&11
IFS= read -r starting_program <&10
cat <&0 >&11 &
inputter=$!

waitforlineoutput "^(\\[Inferior [0-9]+ \\(process [0-9]+\\) exited .*]|^Breakpoint [0-9]+,.*)\$"

kill "$inputter" 2>/dev/null ||:

cat >&11 <<EOF
info registers
quit \$r0
EOF
exec 11<&-
exec 10<&-

wait "$childpid" && ret=$? || ret=$?
if ((ret)); then
	echo "$0: $executable exited with $ret" >&2
fi
exit "$ret"


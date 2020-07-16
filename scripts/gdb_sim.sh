#!/bin/bash
shopt -u lastpipe
set -e -u +o pipefail
export SHELLOPTS

trap_err() {
    echo "Error on line $1" >&2
}
trap 'trap_err $LINENO' ERR

name=$(basename "$0")
usage() {
	cat <<EOF
Usage:
  $name [OPTIONS] executable [arguments...]

Run executable under gdb with 'target sim'.

Options:
  -g <gdb>  Path to the gdb executable.
  -v        Be verbose. Can be used multiple times.
  -i        Interactive connect to gdb after loading the binary.
  -h        Print this help and exit.

Written by Kamil Cukrowski 2019.
EOF
	if (($#)); then
		fatal "$@"
	fi
}

fatal() {
	echo "$name: ERROR:" "$*" >&2
	exit 2
}

# main() ###################################

args=$(getopt -n "gdb_sim.sh" -o vg:ih -- "$@")
eval set -- "$args"
gdb=""
verbose=false
interactive=false
while (($#)); do
	case "$1" in
		-g) gdb="$2"; shift; ;;
		-v) if $verbose; then set -x; fi; verbose=true; ;;
		-i) interactive=true; ;;
		-h) usage; exit 0; ;;
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
	fatal "$executable: Missing executable permission"
fi

# Find proper gdb to run executable with, if not specified.
if [[ -z "$gdb" ]]; then
	fileinfo=$(file "$executable")
	filemachine=$(readelf -h "$executable" | awk '/Machine:/{print $2}')
	machine=$(uname -m)
	case "$machine" in
	x86_64)
		case "$fileinfo" in
		*x86_64*) gdb=gdb; ;;
		*"ARM, EABI"*) gdb=arm-none-eabi-gdb; ;;
		esac
	esac

	if ! hash "$gdb" >/dev/null; then
		fatal "Could not found matching gdb executable"
	fi
else
	if ! hash "$gdb" >/dev/null; then
		fatal "$gdb: command not found"
	fi
fi

# Create temporary directory
umask 077
tmp=$(mktemp -d)
trap_exit() {
	kill "${childpid:-}" "${inputterpid:-}" >/dev/null 2>&1 ||:
	rm -fr "$tmp" ||:
}
trap 'trap_exit' EXIT

in="$tmp/in" out="$tmp/out"
mkfifo "$in" "$out"

# Start the gdb process in the background
gdbchild() {
	if (($# == 0)); then
		echo "gdb_sim.sh: gdbchild: ERROR: Wrong number of arguments" >&2
		exit 1
	fi
	declare -g verbose
	if "$verbose"; then
		/bin/printf "%q\n" "+" "$@" | paste -sd' ' >&2
		stdbuf -oL tee >(sed -u 's/^/< /' >&2) |
		stdbuf -oL "$@" 2>&1 |
		stdbuf -oL tee >(sed -u 's/^/> /' >&2)
		ret=${PIPESTATUS[1]}
		echo gdb exited >&2
		exit "$ret"
	else
		stdbuf -oL "$@" 2>&1
	fi
}
gdbchild "$gdb" -quiet --args "$executable" "${arguments[@]}" <"$out" >"$in" &
childpid=$!
exec 11> "$out"
exec 10< "$in"

# Unique uuid to grep for from gdb output
# to know where does the executable start from
uuid=68ba305a-5761-4afb-bff8-079ea0619b4f

# Gdb startup configuration.
cat >&11 <<'EOF'
set confirm off
target sim
load
break _exit
break _kill
EOF

if "$interactive"; then
	echo "::: Starting interactive gdb session :::"
    cat <&10 11>&- >&1 &
    cat >&11 10>&- <&0
    exit
fi

# Run the executable.
echo 'run' >&11

# Wait for line Starting program with a maximum timeout we give to gdb
ret=0
out=$(
	timeout 10 sed -E -u '
		# This means that we could not start executable, most probably
		# because the architecture of executable does
		# not match architecture of debugger.
		/unable to create simulator instance/q1
		# Error from "load"
		/No executable file specified/q1
		# Executable started successfully
		/Starting program: /q
	' <&10) || ret=$?
if ((ret != 0)); then
	if ((ret == 124)); then
		fatal "Could not start gdb: timeouted: $out"
	else
		fatal "Could not start gdb: $out"
	fi
fi

# Tie standard input with input to gdb,
# so that we can write in terminal something to the program.
while IFS= read -r line; do printf "%s\n" "$line"; done >&11 10>&- <&0 &
inputterpid=$!
# Close stdin, it's no longer needed.
exec 0<&-

# Wait for specified string that tells us the execution ended.
# While at the same time output everything that is inputted.
{ exit_reason=$(
	sed -n -u '
	# Gdb write an empty line before exiting,
	# so we will buffer empty lines in hold space.
	/^$/{s/.*/was empty line/;h;d;N}
	# Quit if end of execution is found.
	/\[Inferior [0-9]\+ (process [0-9]\+) exited \(.*\)]$/b end
	/Breakpoint [0-9]\+, 0x[[:xdigit:]]\+ in \([^[:blank:]]*\) ()$/b end
	# If we did not quit, but we buffered empty line print it.
	x;/was empty line/{s/.*//;p};x
	# Print the line.
	p
	# Start again
	d

	: end {
		# Add a newline on the matched part.
		s//\n\1/
		# Output anything before the string if there is anything.
		h
		s/\n.*//
		/^$/!p
		g
		# Replace the newline for uuid and print it.
		s/.*\n/'"$uuid"'/
		p
		# Quit
		q
	}
	' <&10 |
	# Output everything except the uuid line on terminal
	stdbuf -oL tee >(grep --line-buffered -v "^$uuid" >&3) |
	# Grab the uuid line
	sed '/^'"$uuid"'/!d; s///'
) ;} 3>&1

# Inputter is not loger needed - should be killed,
# but it could have been killed already by EOF from input.
kill "$inputterpid" 2>/dev/null ||:
wait "$inputterpid" ||:

# Make sure user writes a newline, so out inputs are properly parsed.
echo >&11
# info
if "$verbose"; then
	echo "+ exit_reason=$exit_reason"
	echo 'info registers' >&11
fi

# Exit gdb depending on exit reason
case "$exit_reason" in
_exit)
	# https://github.com/bminor/newlib/blob/master/libgloss/arm/_kill.c#L18
	# Get the status argument
	echo 'quit $r1' >&11
	;;
_kill)
	# https://github.com/bminor/newlib/blob/master/libgloss/arm/_kill.c
	# Get the second argument
	echo 'quit $r2' >&11
	;;
successfully)
	# All ok
	;;
*)
	fatal "Unknown exit reason aquired from gdb: $exit_reason"
	;;
esac

# Close input - send EOF to gdb
exec 11>&-
# Ignore everything from output to /dev/null
# Gdb should quit and close the pipe from his side
# We give 1 second for gdb to do that
timeout 1 cat <&10- >/dev/null
exec 10<&-

# Get gdb return status
ret=0
wait "$childpid" || ret=$?
wait

if ((ret)); then
	# Sadly newlib signal numbers and linux are not aligned
	strsignal() {
		case "$1" in
		6) echo 'Aborted'; ;;
		11) echo 'Segmentation fault'; ;;
		esac
	}

	if [[ "$exit_reason" = "_kill" ]]; then
		strsignal=$(strsignal "$ret")
		echo "$(basename "$0"): Signal $ret: ${strsignal:+$strsignal }$executable" >&2
		# If signal was the reasin, add 128 just like the shell.
		if ((ret < 128)); then
			ret=$((128 + ret))
		fi
	#else
	#	echo "$0: $executable exited with $ret" >&2
	fi
fi

exit "$ret"


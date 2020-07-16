#!/bin/bash

tmp=$(mktemp)
trap 'rm "$tmp"' EXIT
args=("$@")
run() {
	arm-none-eabi-gcc -ggdb3 -O0 -specs=rdimon.specs -xc -o "$tmp" - <<EOF
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
int func(int a, int b) {
	// no newline - make it harder to filter
	printf("hello");
}
int main() {
	func(0x12456, 0x6789);
	$1
}
EOF
	set -o pipefail
	{ out=$(
		$(dirname "$0")/gdb_sim.sh "${args[@]}" "$tmp" |
		tee >(cat >&3)
	) ;} 3>&1
	ret=$?

	printf "%-20s -> $ret" "$1"
	if [[ -n "$2" ]] && (($2 != ret)); then
		printf " ERROR: $2 != $ret"
	fi
	if ! <<<"$out" grep -q hello; then
		printf " NO HELLO"
	fi
	printf "\n"
}

# Note - There should be no hello when stdout is not flushed

run '/*empty*/' 0
run 'exit(0);' 0
run 'abort();' $((128 + 6))
run 'kill(0, SIGABRT);' $((128 + 6))
run 'raise(SIGABRT);' $((128 + 6))
run 'kill(0, SIGSEGV);' $((128 + 11))
run 'raise(SIGSEGV);' $((128 + 11))
run 'kill(0, SIGUSR1);' $((128 + 30))
run 'raise(SIGUSR1);' $((128 + 30))
run 'exit(1);' 1
run 'exit(42);' 42
run '_Exit(0);' 0
run '_Exit(1);' 1
run '_Exit(42);' 42
run '_exit(0);' 0
run '_exit(1);' 1
run '_exit(42);' 42
run '*(volatile int*)0 = 42;'


#!/bin/bash

if (($# == 0)); then
	cd "$(dirname "$0")/../gen"
else
	cd "$1"
fi

out=$(
	find . -type f -path '*.[ch]' |
	xargs grep -n '^[[:blank:]]*#[[:blank:]]*include[[:blank:]]*.*<yio' |
	sort
)

if [[ -n "$out" ]]; then
	printf "%s\n" "$out"
	exit 1
fi


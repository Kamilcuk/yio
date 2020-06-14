#!/bin/bash

rp() {
	realpath -s -e --relative-to="$PWD" "$(dirname "$0")"/../"$1"
}
cmd=(m4 -P -I$(rp m4) -I$(rp gen) -I$(rp test/m4) lib.m4 -)
set -x
"${cmd[@]}"


#!/bin/bash

if ((!$#)); then
	set -- -
fi
rp() {
	realpath -s -e --relative-to="$PWD" "$(dirname "$0")"/../"$1"
}
cmd=(m4 -P "-Dm4_SRCDIR=$(rp src)" "-I$(rp m4)" "-I$(rp gen)" "-I$(rp test/m4)")
# Load config if possible.
if [[ -e "$(dirname "$0")/../gen/m4/yio_config.m4" ]]; then
	cmd+=("-I$(rp gen/m4/yio_config.m4)")
fi
cmd+=(lib.m4 "$@")
set -x
"${cmd[@]}"


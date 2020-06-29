#!/bin/sh
set -eu

mkdir -p "$1"
d=$(readlink -f "$1")
cd "$(dirname "$0")/.."

./scripts/list_git_files_only.sh -z |
if hash rsync >/dev/null 2>&1; then
	( set -x
	rsync -0 --files-from=/dev/stdin -a . "$d"
	)
else 
	( set -x
	xargs -0 -t cp --parents -a -t "$d"
	)
fi


#!/bin/bash
set -euo pipefail

cd "$(dirname "$0")"/..

mkdir -p "$1"

{
	echo .git
	git ls-files . --exclude-standard --others --cached
} |
if hash rsync >/dev/null 2>&1; then
	( set -x
	rsync --files-from=/dev/stdin -av . "$1"
	)
else 
	( set -x
	xargs -t -d'\n' cp --parents -a -t "$1"
	)
fi


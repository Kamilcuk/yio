#!/bin/bash
set -euo pipefail

mkdir -p "$1"
d=$(readlink -f "$1")
cd "$(dirname "$0")/.."

{
	echo .git
	git ls-files . --exclude-standard --others --cached
} |
if hash rsync >/dev/null 2>&1; then
	( set -x
	rsync --files-from=/dev/stdin -av . "$d"
	)
else 
	( set -x
	xargs -t -d'\n' cp --parents -a -t "$d"
	)
fi


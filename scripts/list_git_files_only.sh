#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
tmp=$(mktemp)
trap 'rm -rf "$tmp"' EXIT

zopt=
zfindopt=
if [ "${1:-}" = "-z" ]; then
	zopt=-z
	zfindopt=-print0
fi

git ls-files $zopt --deleted | sort $zopt > "$tmp"
{
	find .git $zfindopt
	git ls-files $zopt . --exclude-standard --others --cached 
} |
sort $zopt |
join $zopt -v 1 - "$tmp"



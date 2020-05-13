#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")"/../src
find . -type f '(' -name '*.h' -o -name '*.c' ')' |
while IFS= read -r l; do
	to=$(basename "$l")
	echo "Fixing $l to $to"
	( set -x
	sed -i -e 's/^ \* @file .*/ \* @file '"$to"'/' "$l"
	)
done


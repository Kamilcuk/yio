#!/bin/bash

fixit() {
	local file
	file=$1
	filename=$(basename "$1")
	cur=$(sed '/ *\* *@file */!d; s///' "$file")
	if [[ "$cur" != "$filename" ]]; then
		echo "Fixing $1"
		/bin/printf "%q " sed -i .bak 's/ *\* *@file *.*/ * @file '"$filename"'/' "$file"
		echo
	fi
}
export -f fixit

find "$(dirname "$0")/../src" -type f '(' -name '*.c' -o -name '*.h' ')' |
xargs -d '\n' -n1 bash -c 'fixit "$@"' _


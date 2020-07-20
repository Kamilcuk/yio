#!/bin/bash
set -euo pipefail
export SHELLOPTS

cd $(dirname "$0")/..

newline_at_eof()
{
    if [ -z "$(tail -c 1 "$1")" ]
    then
        echo "Newline at end of file!"
    else
        echo "No newline at end of file!"
    fi
}

list=$(
	find src -type f |
	while IFS= read -r line; do
		tmp=$(tail -c 1 "$line" | xxd -p)
		if [[ "$tmp" != '0a' ]]; then
			printf "%s\n" "$line"
		fi
	done
)

if [[ -z "$list" ]]; then
	exit
fi

echo "Following files do not have a newline on the end:"
printf "%s\n" "$list"


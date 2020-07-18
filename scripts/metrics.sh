#!/bin/bash
set -euo pipefail
DIR="$(dirname "$(readlink -f "$0")")/.."
if ((!$#)); then
	cat <<EOF
Usage: $(basename $0) builddir
EOF
fi

cd "$1"

out+=$(find . -maxdepth 2 -type f -name libyio.a |
	xargs size -G |
	(
	 	read -r;
		#printf "%s\n" "$REPLY";
		sort -k6
	) |
	sed 's/^[[:space:]]*\([0-9]*\)[[:space:]]*\([0-9]*\)[[:space:]]*\([0-9]*\)[[:space:]]*\([0-9]*\)[[:space:]]*\([^ ]*\).*/'\
'\5.total \4/')

out+=$'\n'
out+="Total_Test_time "
out+=$(ctest . | tail -n1 | sed 's/.* \([^ ]*\) sec$/\1/')

cat <<<"$out"


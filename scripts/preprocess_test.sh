#!/bin/bash
set -euo pipefail
cd "$(dirname "$(readlink -f "$0")")"/../src
a=$(mktemp -p .)
b=$(mktemp)
trap 'rm -f "$a" "$b"' EXIT
printf "%s\n" "$@" > "$a"
./preprocess.py -S . "$a" "$b"
cat "$a"
echo ---
cat "$b"



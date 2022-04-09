#!/bin/bash
set -euo pipefail
a=$(readlink -f "$1")
set -x
gcc -E -I "$1" "$a"

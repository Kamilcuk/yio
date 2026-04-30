#!/bin/bash
set -xeuo pipefail

if ! hash r2; then
  exec nix shell nixpkgs#radare2 nixpkgs#graphviz --command "$0" "$@"
fi

DIR=$(dirname "$(readlink -f "$0")")
cd "$DIR"
INPUT_FILE=${1:-../_build/DefaultDebug/lib/libyiod.a}
OUTPUT_BASE=$(basename "$INPUT_FILE")
DOT_FILE="../_build/${OUTPUT_BASE}.dot"
PNG_FILE="../_build/${OUTPUT_BASE}.png"

echo "Analyzing $INPUT_FILE..."

# r2 commands:
# aaa: Analyze all (functions, symbols, etc.)
# agC: Export global call graph in DOT format
# q: Quit
r2 -A -q -c "aaa; agC > $DOT_FILE" "$INPUT_FILE"

if [ -f "$DOT_FILE" ]; then
    echo "DOT file generated: $DOT_FILE"

    # Convert DOT to PNG using Graphviz
    if command -v dot &> /dev/null; then
        dot -Tpng "$DOT_FILE" -o "$PNG_FILE"
        echo "Graph rendered: $PNG_FILE"
    else
        echo "Graphviz (dot) not found. Use 'xdot $DOT_FILE' to view."
    fi
else
    echo "Failed to generate call graph."
    exit 1
fi

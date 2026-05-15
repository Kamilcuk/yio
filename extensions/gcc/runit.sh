#!/bin/bash
set -e
# DIR is the absolute path to the directory containing this script
DIR=$(dirname "$(readlink -f "$0")")
# BUILD_DIR is relative to the script directory
BUILD_DIR="build"

export TESTPROGRAM="$1"

# Change to the script directory to keep paths relative
cd "${DIR}"

# Run cmake and build without suppressing output
cmake -B "${BUILD_DIR}" -S "."
cmake --build "${BUILD_DIR}" --target yio_plugin_testprogram

# Run the generated test program
"./${BUILD_DIR}/yio_plugin_testprogram"

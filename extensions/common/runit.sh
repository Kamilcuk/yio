#!/bin/bash
set -euo pipefail
DIR=$(readlink -f "$(dirname "$0")")
BUILD_DIR=$DIR/build
export TESTPROGRAM="$*"
cd "${DIR}"
set -x
cmake -B "${BUILD_DIR}" -S .
cmake --build "${BUILD_DIR}" --target testprogram
"${BUILD_DIR}/testprogram"

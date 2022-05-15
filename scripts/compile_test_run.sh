#!/bin/bash
set -xeuo pipefail
make TESTPROGRAM="$*" testprogram

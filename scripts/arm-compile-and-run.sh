#!/bin/bash
set -xeuo pipefail
./innix.sh env PRESET=arm TESTPROGRAM="$*" make .build_yio_testprogram
./innix.sh qemu-arm -cpu cortex-m3 -L /usr/arm-none-eabi _build/ArmDebug/bin/yio_testprogram

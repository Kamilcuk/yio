#!/bin/bash
. "$(dirname "$(readlink -f "$0")")"/gdb_sim.sh -g arm-none-eabi-gdb "$@"

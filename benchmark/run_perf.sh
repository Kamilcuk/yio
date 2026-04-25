#!/bin/bash
perf stat -e instructions,cycles,branches,branch-misses,context-switches,page-faults,L1-dcache-load-misses "$@"

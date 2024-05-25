#!/bin/bash
set -x
env LC_ALL=C ,watchfile test.m4 'timeout -k 1 -v 1 m4 -d -d -d -D m4_DEBUG -D m4_TEST -gPI . -L 100 -E -E test.m4 2>&1'

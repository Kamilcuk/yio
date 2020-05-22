#!/bin/bash
cd "$(dirname "$0")"/..
set -x
m4 -P -Im4 -Igen -Itest/m4 lib.m4 -


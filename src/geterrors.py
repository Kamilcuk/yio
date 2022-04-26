#!/usr/bin/env python3

"""
A small script that will extract all calls to _yIO_ERROR within all source files
in current source directory, and from these calls it will generate two files:
.c and .h file given as first and second argumetns with enum-ish definition
and array of strings definition.
It is used as part of CMake scripts to generate error messages.
"""

import os
import re
import sys

rematcherr = re.compile(r'_yIO_ERROR\(.*"')
rereplace = re.compile(r'.*_yIO_ERROR\( *([A-Z_]*) *, *(".*") *\).*')
dir = os.path.dirname(__file__)
errors = []
for root, dirs, files in os.walk(dir):
    for filename in files:
        if not filename.endswith(".c"):
            continue
        file = os.path.join(root, filename)
        for line in open(file):
            line = line.strip()
            if rematcherr.search(line):
                res, no = rereplace.subn(r"\1 \2", line)
                if no != 1:
                    raise Exception("Invalid _yIO_ERROR no=" + no + " file=" + file)
                res = res.split(" ", 1)
                errors += [("YIO_ERROR_" + res[0], res[1])]

enumout = "/* @file */\n"
for k, v in errors:
    enumout += "\t" + k + ",  /// " + v + "\n"

msgout = "/* @file */\n"
for k, v in errors:
    msgout += "\t" + v + ",  // " + k + "\n"


def mywrite(idx, str):
    if len(sys.argv) > idx:
        file = sys.argv[idx]
        if not os.path.exists(file) or open(file, "r").read() != str:
            print(str, file=open(file, "w"))
            print("written to " + file)
        else:
            print("nochanges to " + file)
    else:
        print(str)


mywrite(1, enumout)
mywrite(2, msgout)

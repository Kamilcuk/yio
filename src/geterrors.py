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
from preprocess import save_if_changed, LL


def get_all_errors_from_sources():
    rereplace = re.compile(
        r'_yIO_ERROR\s*\(\s*(YIO_ERROR_[A-Z_]+)\s*,\s*(".*")\s*\)',
        flags=re.MULTILINE,
    )
    dir = os.path.dirname(__file__)
    errors = []
    for root, dirs, files in os.walk(dir):
        for filename in files:
            if not filename.endswith(".c"):
                continue
            file = os.path.join(root, filename)
            content = open(file).read()
            for res in rereplace.findall(content):
                errors += [(res[0], res[1])]
    return errors


def check_duplicates(arr, name):
    dupl = [el for el in arr if arr.count(el) > 1]
    if len(dupl) > 0:
        print("Duplicated " + name + ": " + str(dupl), file=sys.stderr)
        sys.exit(1)


def prepare_sources(errors):
    enumout = "/* @file */\n"
    for k, v in errors:
        enumout += "\t" + k + ",  /// " + v + "\n"
    msgout = "/* @file */\n"
    for k, v in errors:
        msgout += "\t" + v + ",  // " + k + "\n"
    return (enumout, msgout)


def write_to_argv_on_change(idx, str):
    if len(sys.argv) <= idx:
        print(str)
        return
    file = sys.argv[idx]
    save_if_changed(str, file, file)


if __name__ == "__main__":
    # LL.setLevel("DEBUG")
    LL.setLevel("INFO")
    errors = get_all_errors_from_sources()
    check_duplicates([k for k, v in errors], "enums")
    check_duplicates([v for k, v in errors], "messages")
    enumout, msgout = prepare_sources(errors)
    write_to_argv_on_change(1, enumout)
    write_to_argv_on_change(2, msgout)

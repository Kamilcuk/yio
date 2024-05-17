#!/usr/bin/env python3

import argparse
import logging
import os
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import List

from preprocess import save_if_changed

log = logging.getLogger(__name__)


@dataclass
class Err:
    enum: str
    msg: str

    def __post_init_(self):
        assert self.enum.startswith(
            "YIO_ERROR_"
        ), f"{self.enum}: Argument to YYIO_ERROR does not start with YIO_ERROR"


def fatal(str, *args):
    print(str.format(*args), file=sys.stderr)
    sys.exit(1)


def get_all_errors_from_sources():
    rereplace = re.compile(
        r'YYIO_ERROR\s*\(\s*(.*)\s*,\s*(".*")\s*\)\s*;',
        flags=re.MULTILINE,
    )
    dir = os.path.dirname(__file__)
    errors: List[Err] = []
    for path in Path(dir).glob("**/*.c"):
        for line in open(path).read():
            res = rereplace.findall(line)
            if res:
                errors += [Err(*res)]
    return errors


def check_duplicates(arr, name):
    dupl = [el for el in arr if arr.count(el) > 1]
    assert len(dupl) == 0, f"Duplicated {name}: {dupl}"


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


def main():
    parser = argparse.ArgumentParser(
        description="""
    A small script that will extract all calls to YYIO_ERROR within all source files
    in current source directory, and from these calls it will generate two files:
    .c and .h file given as first and second argumetns with enum-ish definition
    and array of strings definition.
    It is used as part of CMake scripts to generate error messages.
    """
    )
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument(
        "file1",
        help="output the integers for enum",
        default=sys.stdout,
        type=argparse.FileType("w"),
    )
    parser.add_argument(
        "file2",
        help="output the strings of errors",
        default=sys.stdout,
        type=argparse.FileType("w"),
    )
    args = parser.parse_args()
    logging.basicConfig(level=logging.DEBUG if args.verbose else logging.INFO)
    #
    errors = get_all_errors_from_sources()
    check_duplicates([e.enum for e in errors], "enums")
    check_duplicates([e.msg for e in errors], "messages")
    enumout, msgout = prepare_sources(errors)
    write_to_argv_on_change(1, enumout)
    write_to_argv_on_change(2, msgout)


if __name__ == "__main__":
    main()

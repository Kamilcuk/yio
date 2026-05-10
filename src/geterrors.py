#!/usr/bin/env python3

import argparse
import logging
import os
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import List, Set

from preprocess import save_if_changed

log = logging.getLogger(__name__)


@dataclass(frozen=True)
class Err:
    enum: str
    msg: str

    def __post_init__(self):
        assert self.enum.startswith("YIO_ERROR_"), (
            f"{self.enum}: Argument to YYIO_ERROR does not start with YIO_ERROR"
        )


def get_all_errors_from_sources(paths: List[Path]) -> List[Err]:
    # Match YYIO_ERROR(ENUM, "message")
    rereplace = re.compile(
        r'YYIO_ERROR\s*\(\s*([A-Z0-9_]+)\s*,\s*("[^"]*")\s*\)',
    )
    errors: Set[Err] = set()
    for path in paths:
        try:
            content = path.read_text()
            matches = rereplace.findall(content)
            for enum, msg in matches:
                errors.add(Err(enum, msg))
        except Exception as e:
            log.warning(f"Could not read {path}: {e}")

    # Sort for deterministic output
    return sorted(list(errors), key=lambda x: x.enum)


def prepare_sources(errors: List[Err]):
    enumout = "/* @file */\n"
    for err in errors:
        enumout += f"\t{err.enum},  /// {err.msg}\n"
    msgout = "/* @file */\n"
    for err in errors:
        msgout += f"\t{err.msg},  // {err.enum}\n"
    return (enumout, msgout)


def write_to_file_on_change(file_path, str_content):
    save_if_changed(str_content, Path(file_path))


def main():
    parser = argparse.ArgumentParser(
        description="""
    A small script that will extract all calls to YYIO_ERROR within all source files
    passed as arguments, and from these calls it will generate two files:
    .c and .h file given as --enum and --msg arguments with enum-ish definition
    and array of strings definition.
    It is used as part of CMake scripts to generate error messages.
    """
    )
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument(
        "--enum",
        required=True,
        help="output the integers for enum",
    )
    parser.add_argument(
        "--msg",
        required=True,
        help="output the strings of errors",
    )
    parser.add_argument(
        "sources",
        nargs="+",
        help="source files to scan for errors",
    )
    args = parser.parse_args()
    logging.basicConfig(level=logging.DEBUG if args.verbose else logging.INFO)
    #
    errors = get_all_errors_from_sources([Path(s) for s in args.sources])
    enumout, msgout = prepare_sources(errors)
    write_to_file_on_change(args.enum, enumout)
    write_to_file_on_change(args.msg, msgout)


if __name__ == "__main__":
    main()

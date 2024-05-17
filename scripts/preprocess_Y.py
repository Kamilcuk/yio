#!/usr/bin/env python3


import argparse
import logging
import re
import sys

log = logging.getLogger(__name__)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument(
        "infile",
        nargs="?",
        type=argparse.FileType("r"),
        default=sys.stdin,
        help="Input file",
    )
    parser.add_argument(
        "outfile",
        nargs="?",
        type=argparse.FileType("w"),
        default=sys.stdout,
        help="Output file",
    )
    args = parser.parse_args()
    logging.basicConfig(level=logging.DEBUG if args.verbose else logging.INFO)
    return args


def sub_Y(data: str) -> str:
    for m in reversed(
        list(re.finditer(r"\(Y\((.*?)\)\);\n", data, re.MULTILINE | re.DOTALL))
    ):
        log.debug("->", f"{m[0]!r}")
        fmt = m[1]
        argsstr: str = ""
        for r in reversed(
            list(re.finditer(r"(?<!{){(?P<var>[^{}:]*)(?P<spec>:[^{}]*)?}", fmt))
        ):  # }
            fmt = (
                fmt[: r.start()]
                + (r["var"] + "=" if r["spec"] and r["spec"].startswith(":=") else "")
                + "{"
                + (r["spec"] or "")
                + "}"
                + fmt[r.end() :]
            )
            argsstr += "," + r["var"]
        repl = "(" + fmt + argsstr + ");\n"
        log.debug("<-", f"{repl!r}")
        data = data[: m.start()] + repl + data[m.end() :]
    return data


if __name__ == "__main__":
    args = parse_args()
    data = args.infile.read()
    args.infile.close()
    data = sub_Y(data)
    args.outfile.write(data)
    args.outfile.close()

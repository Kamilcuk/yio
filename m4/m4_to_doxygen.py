#!/usr/bin/env python

import re
import sys
from pathlib import Path


def matched_len(data: str):
    assert len(data)
    cntq = 1
    i = 0
    for i, c in enumerate(data):
        if c == "«":
            cntq += 1
        elif c == "»":
            cntq -= 1
        if cntq == 0:
            break
    return i


def idx_to_lineno(data, idx):
    return sum(1 for c in data[:idx] if c == "\n")

def main():
    matches = []
    with Path(sys.argv[1]).open() as f:
        data = f.read()
    for i in re.finditer(
        r"(?:^|\n)m4_define[^(]*\((\s*)«([^»]*)»\s*,\s*«",
        data,
        re.DOTALL,
    ):  # ))
        length = matched_len(data[i.end() :])
        start = i.end() - 1
        end = i.end() + length + 1
        ext = data[start : end]
        # print("|", ext[:400], "|")
        # print(f"---- {len(ext.splitlines())} {end}")
        define = f"#define {i.group(1)}{i.group(2)} {ext}"
        define = define.replace("\n", "  \\\n")
        matches += [(idx_to_lineno(data, start), idx_to_lineno(data, end), define)]
    for lineno, line in enumerate(data.splitlines()):
        m = next(( m for m in matches if m[0] <= lineno < m[1]), None)
        if m:
            idx = lineno - m[0]
            print(m[2].splitlines()[idx])
        else:
            print(line)

if __name__ == "__main__":
    main()

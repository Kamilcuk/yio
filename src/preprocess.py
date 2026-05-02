#!/usr/bin/env python3
"""
Jinja2-based preprocessor for Yio Iconic Output Library.

This script processes C/C++ templates using Jinja2 to generate type-safe
formatting code. It supports batch processing of multiple tasks, dependency
tracking (via .depfile), and concurrent execution.
"""

import argparse
import concurrent.futures
import logging
import re
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple, Union

import jinja2
import jinja2.ext

log = logging.getLogger(Path(__file__).name)

# j_FLOATS configuration
_j_FLOATS_RAW = [
    ["N", "G", "type", "math", "strto"],
    ["f", "s", "float", "f", "f"],
    ["d", "s", "double", "", "d"],
    ["l", "s", "long double", "l", "ld"],
    ["f16", "f", "_Float16", "f16", "f16"],
    ["f32", "f", "_Float32", "f32", "f32"],
    ["f64", "f", "_Float64", "f64", "f64"],
    ["f128", "f", "_Float128", "f128", "f128"],
    ["f32x", "fx", "_Float32x", "f32x", "f32x"],
    ["f64x", "fx", "_Float64x", "f64x", "f64x"],
    ["f128x", "fx", "_Float128x", "f128x", "f128x"],
    ["d32", "d", "_Decimal32", "d32", "d32"],
    ["d64", "d", "_Decimal64", "d64", "d64"],
    ["d128", "d", "_Decimal128", "d128", "d128"],
    ["d32x", "dx", "_Decimal32x", "d32x", "d32x"],
    ["d64x", "dx", "_Decimal64x", "d64x", "d64x"],
    ["d128x", "dx", "_Decimal128x", "d128x", "d128x"],
]

j_FLOATS: List[Dict[Union[int, str], str]] = [
    {1: x[0], **{v: k for v, k in zip(_j_FLOATS_RAW[0], x) if v != "N"}}
    for x in _j_FLOATS_RAW[1:]
]


def j_one_to_n(*args: Any) -> range:
    """Just like range, but start from 1 by default, and end exactly on end."""
    if len(args) == 3:
        raise RuntimeError("too many arguments")
    if len(args) == 1:
        args = (1, args[0])
    vals = [int(x) for x in args]
    return range(vals[0], vals[1] + 1)


def j_match(value: Any, regex: str) -> bool:
    return bool(re.match(regex, str(value)))


def j_fatal(value: Any = "fatal error") -> None:
    raise Exception(str(value))


def j_is_power_of_two(n: int) -> bool:
    return n > 0 and (n & (n - 1)) == 0


class MyEnvironment(jinja2.Environment):
    def __init__(
        self, srcdirs: List[str], cachedir: Optional[str], defines: Dict[str, str]
    ):
        super().__init__(
            loader=jinja2.FileSystemLoader(srcdirs),
            trim_blocks=True,
            lstrip_blocks=True,
            keep_trailing_newline=True,
            extensions=["jinja2.ext.do"],
            bytecode_cache=jinja2.FileSystemBytecodeCache(cachedir)
            if cachedir
            else None,
        )
        self.globals.update(
            {
                "j_FLOATS": j_FLOATS,
                "j_MAX_ARGS": int(defines.get("j_MAX_ARGS", "32")),
                "j_MAX_CUSTOM_SLOTS": int(defines.get("j_MAX_CUSTOM_SLOTS", "100")),
                "j_BITINT_MAXWIDTH": int(defines.get("j_BITINT_MAXWIDTH", "128")),
                "j_one_to_n": j_one_to_n,
                "j_match": j_match,
                "j_fatal": j_fatal,
                "j_is_power_of_two": j_is_power_of_two,
            }
        )
        self.filters.update(
            {
                "j_one_to_n": j_one_to_n,
                "j_match": j_match,
            }
        )
        self.dependencies: List[str] = []

    def preprocess(
        self, source: str, name: Optional[str], filename: Optional[str]
    ) -> str:
        return preprocess_source(source, filename or "")

    def get_template(
        self,
        name: str,
        parent: Optional[str] = None,
        globals: Optional[Dict[str, Any]] = None,
    ) -> jinja2.Template:
        template = super().get_template(name, parent, globals)
        if template.filename and template.filename not in self.dependencies:
            self.dependencies.append(template.filename)
        return template


def preprocess_source(source: str, filename: str) -> str:
    if not (filename.endswith(".c") or filename.endswith(".h")):
        return source
    if "{% from 'library.jinja'" in source:
        return source

    imports = [
        "j_seq",
        "j_seqcomma",
        "j_seqdashcomma",
        "j_APPLY_IN",
        "j_APPLY",
        "j_FOREACHAPPLY",
        "j_FUNC",
    ]
    header = "{% from 'library.jinja' import " + ",".join(imports) + " %}\n"

    doline = "NOLINE" not in source
    lines = []
    for lineno, line in enumerate(source.split("\n")):
        if "#line" in line:
            replacement = f'#line {lineno + 2} "{filename}"' if doline else ""
            lines.append(line.replace("#line", replacement))
        else:
            lines.append(line)
    return header + "\n".join(lines)


def save_if_changed(output: str, outfilename: Path) -> bool:
    if outfilename.exists() and outfilename.read_text() == output:
        return False
    outfilename.parent.mkdir(parents=True, exist_ok=True)
    outfilename.write_text(output)
    return True


def run_task(
    srcdirs: List[str],
    cachedir: Optional[str],
    defines: Dict[str, str],
    source: str,
    output_file: str,
) -> Tuple[str, List[str]]:
    try:
        source_path = Path(source)
        loader_paths = list(srcdirs)
        template_name = source
        if source_path.is_absolute():
            loader_paths.insert(0, str(source_path.parent))
            template_name = source_path.name

        env = MyEnvironment(loader_paths, cachedir, defines)
        template = env.get_template(template_name)

        rendered = template.render()
        header = "// Do not edit! Auto-generated by preprocess.py\n"
        if "NOLINE" not in rendered:
            header += f'#line 1 "{template.filename}"\n'
        save_if_changed(header + rendered, Path(output_file))
        return output_file, list(env.dependencies)
    except Exception:
        log.exception(f"Failed to process {source} -> {output_file}")
        raise


class SerialExecutor(concurrent.futures.Executor):
    def submit(self, fn, *args, **kwargs):
        f = concurrent.futures.Future()
        try:
            f.set_result(fn(*args, **kwargs))
        except Exception as e:
            f.set_exception(e)
        return f


def parse_arguments():
    parser = argparse.ArgumentParser(description="Jinja2-based preprocessor for Yio.")
    parser.add_argument(
        "-S", "--srcdir", default=[], action="append", help="Source directory."
    )
    parser.add_argument(
        "-D", "--define", default=[], action="append", help="Define variables."
    )
    parser.add_argument("-C", "--cachedir", help="Bytecode cache directory.")
    parser.add_argument("-d", "--depfile", help="Dependency file path.")
    parser.add_argument("-j", "--jobs", type=int, default=1, help="Concurrent jobs.")
    parser.add_argument("tasks", nargs="*", help="Source and output file pairs.")
    return parser.parse_args()


def main():
    args = parse_arguments()
    defines = dict(d.split("=", 1) for d in args.define)

    if not args.tasks:
        return

    if len(args.tasks) % 2 != 0:
        raise RuntimeError("Tasks must be provided in pairs (source output)")

    tasks = [args.tasks[i : i + 2] for i in range(0, len(args.tasks), 2)]

    if args.cachedir:
        Path(args.cachedir).mkdir(parents=True, exist_ok=True)

    all_dependencies: Dict[str, List[str]] = {}

    Executor = (
        concurrent.futures.ThreadPoolExecutor if args.jobs > 1 else SerialExecutor
    )
    with Executor(max_workers=args.jobs) as ex:
        futures = [
            ex.submit(run_task, args.srcdir, args.cachedir, defines, t[0], t[1])
            for t in tasks
        ]
        for future in concurrent.futures.as_completed(futures):
            out_file, deps = future.result()
            all_dependencies[out_file] = deps

    if args.depfile:
        dep_path = Path(args.depfile)
        dep_path.parent.mkdir(parents=True, exist_ok=True)
        with dep_path.open("w") as f:
            for out_file, deps in sorted(all_dependencies.items()):
                deps_str = " ".join(
                    sorted([str(Path(d).resolve()).replace(" ", "\\ ") for d in deps])
                )
                f.write(
                    f"{Path(out_file).resolve().as_posix().replace(' ', '\\ ')}: {deps_str}\n"
                )


if __name__ == "__main__":
    main()

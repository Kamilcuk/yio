#!/usr/bin/env python3

import argparse
import datetime
import inspect
import jinja2
import jinja2.ext
import jinja2.meta
import logging
import os
import re
import sys

###############################################################################

log = logging.getLogger(os.path.basename(__file__))

j_FLOATS = [
    # N - "name"
    # G - "group". s = "standard", "f" = floats, "d" = decimals
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

j_FLOATS = [
    # "invert" the table and replace N for 0, so it is j_FLOATS = [ {0:"f", "type": "float", etc..}, ... ]
    {**dict([(0, x[0])]), **{v: k for v, k in zip(j_FLOATS[0], x) if v != "N"}}
    for x in j_FLOATS[1:]
]


def j_range(*args) -> range:
    """Just like range, but start from 1 by default, and end exactly on end, instead of end -1"""
    if len(args) == 3:
        raise RuntimeError("too many arguments")
    if len(args) == 1:
        args = [1, args[0]]
    args = [int(x) for x in args]
    return range(args[0], args[1] + 1)


def j_match(value, regex) -> bool:
    return re.match(regex, str(value))


def j_fatal(value="fatal error"):
    raise Exception(str(value))


def j_frametemplate():
    template = None
    for frameInfo in inspect.stack():
        if frameInfo.frame.f_globals.get("__jinja_template__") is not None:
            template = frameInfo.frame.f_globals.get("__jinja_template__")
            break
    return template


DEPENDENCIES = []


class MyInclude(jinja2.ext.ExprStmtExtension):
    def parse(self, parser):
        node = super().parse(parser)
        return node


class MyEnvironment(jinja2.Environment):
    def get_template(self, name, parent=None, globals=None):
        if name not in DEPENDENCIES:
            DEPENDENCIES.append(name)
        return super().get_template(name, parent, globals)


def prepare_environment(args):
    global DEPENDENCIES
    DEPENDENCIES = [args.source]
    env = MyEnvironment(
        loader=jinja2.FileSystemLoader(args.srcdir),
        trim_blocks=True,
        lstrip_blocks=True,
        keep_trailing_newline=True,
        extensions=[MyInclude],
    )
    defines = {}
    for d in args.define:
        k, v = d.split("=", 1)
        defines[k] = v
    env.globals.update(
        {
            "j_FLOATS": j_FLOATS,
            "j_MLVLS": int(defines.get("j_MLVLS", 32)),
            "j_range": j_range,
            "j_match": j_match,
            "j_fatal": j_fatal,
            "j_frametemplate": j_frametemplate,
        }
    )
    env.filters.update(
        {
            "j_range": j_range,
            "j_match": j_match,
        }
    )
    env.preprocess = lambda source, name, filename: preprocess_source(source, filename)
    return env


def shoulddoline(source):
    return "# NOLINE" not in source


def preprocess_source(source, filename):
    if not filename.endswith(".c") and not filename.endswith(".h"):
        return source

    # If the file already contains from library.jinja, skip adding it
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
    output = "{% from 'library.jinja' import " + ",".join(imports) + " %}\n"

    doline = shoulddoline(source)
    # Replace '#line' by the proper C directive.
    for lineno, line in enumerate(source.split("\n")):
        if "#line" in line:
            replacement = (f'#line {lineno + 2} "{filename}"') if doline else ""
            output += line.replace("#line", replacement) + "\n"
        else:
            output += line + "\n"
    return output


def postprocess(output, infilename, mode):
    log.debug(f"mode={mode}\t{infilename}")
    if mode == "yio":
        # Replace Ω and π.
        output = output.replace("Ω", "").replace("π", "")

        # Replace names.
        names = {
            "TMODEX": 1,
            "TMODE": 1,
            "TMODEN": 1,
            "TCHAR": "char",
            "TINT": "int",
            "TEOF": "EOF",
            "TPRI": '"s"',
        }
        for kk, vv in names.items():
            output = re.sub(r"\b{}\b".format(kk), str(vv), output)

        # Replace function calls
        funcs = {
            "TC": "{}",
            "TFPRINTF": "fprintf({})",
            "TISDIGIT": "isdigit((unsigned char){})",
            "TISXDIGIT": "isxdigit((unsigned char){})",
            "TSTRCHR": "strchr({})",
            "TSTRCMP": "strcmp({})",
            "TSTRLEN": "strlen({})",
        }
        for kk, vv in funcs.items():
            output = re.sub(
                r"\b{}\b\s*\(([^\)]*)\)".format(kk),
                str(vv).replace("{}", r"\1"),
                output,
            )
        # Replace _yIO_TYPE_Y*IO by 1
        output = re.sub("_yIO_TYPE_" + mode.upper(), "1", output)
    elif mode == "none":
        output = output.replace("Ω", "").replace("π", "")

    header = f"""\
// Do not edit this file! This file was auto-generated by preprocess.py at {datetime.datetime.now()}
// To change this file, edit the source.
// mode = {mode}
#line 1 "{infilename}"
"""
    output = header + output
    return output


def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("-S", "--srcdir", default=[], action="append")
    parser.add_argument("-D", "--define", default=[], action="append")
    parser.add_argument("--cachedir")
    parser.add_argument("--depfile")
    parser.add_argument("-m", "--mode", default="yio", choices=["none", "yio"])
    parser.add_argument("--debug", action="store_true")
    parser.add_argument("source")
    parser.add_argument("output")
    return parser.parse_args()


def save_if_changed(output, outfilename, infilename):
    if os.path.exists(outfilename):
        with open(outfilename, "r") as f:
            if f.read() == output:
                log.debug(f"No changes in {outfilename}")
                return
    log.info(f"Writing {outfilename}")
    os.makedirs(os.path.dirname(outfilename), exist_ok=True)
    with open(outfilename, "w") as f:
        f.write(output)


def depfile_path(path):
    return os.path.realpath(path).replace(" ", "\\ ")


def generate_depfile(depfile, env, infilename, outfilename):
    if depfile is None:
        return
    log.debug(f"Writing depfile {depfile}")
    os.makedirs(os.path.dirname(depfile), exist_ok=True)
    with open(depfile, "w") as f:
        print(
            depfile_path(outfilename)
            + ":"
            + "".join([" " + depfile_path(dd) for dd in DEPENDENCIES]),
            file=f,
        )


###############################################################################

if __name__ == "__main__":
    args = parse_arguments()
    env = prepare_environment(args)

    mode = args.mode
    ttemplate = env.get_template(args.source)
    infilename = ttemplate.filename
    output = ttemplate.render(
        {
            "MODE": 1 if mode == "yio" else -1,
            "MODEX": 3 if mode == "yio" else -1,
            "TMODE": 1 if mode == "yio" else -1,
            "TMODEX": 3 if mode == "yio" else -1,
        }
    )

    output = postprocess(output, infilename, mode)
    outfilename = args.output
    save_if_changed(output, outfilename, outfilename)
    generate_depfile(args.depfile, env, infilename, outfilename)

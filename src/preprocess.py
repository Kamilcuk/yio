#!/usr/bin/env python3


import argparse
import datetime
import inspect
import logging
import os
import re
<<<<<<< Updated upstream
from dataclasses import dataclass
||||||| Stash base
=======
from typing import Set

import jinja2
import jinja2.ext
import jinja2.meta
>>>>>>> Stashed changes

import jinja2.ext

log = logging.getLogger(os.path.basename(__file__))
DIR = os.path.dirname(__file__)

###############################################################################
# Internal define variables and functions exported to jinja

# fmt: off
template_data = {
    "mode":  ["yio", "ywio", "yc16io", "yuio", ],
    "omega": ["",    "W",    "C16",    "U", ],
    "pi":    ["",    "w",    "c16",    "u", ],
    "names": {
        "TMODEX": [1,       2,         3,            3, ],
        "TMODE":  [1,       2,         3,            4, ],
        "TMODEN": [1,       2,         3,            4, ],
        "TCHAR":  ["char",  "wchar_t", "uint16_t",   "uint32_t", ],
        "TINT":   ["int",   "wint_t",  "uint16_t",   "uint32_t", ],
        "TEOF":   ["EOF",   "WEOF",    "UINT16_MAX", "UINT32_MAX", ],
        "TPRI":   ["\"s\"", "\"ls\"",  "\"lU\"",     "\"llU\"", ],
    },
    "funcs": {
        "TC":        ["{}",                          "L{}",           "u{}",              "U{}", ],
        "TFPRINTF":  ["fprintf({})",                 "fprintf({})",   "ulc_fprintf({})",  "ulc_fprintf({})", ],
        "TISDIGIT":  ["isdigit((unsigned char){})",  "iswdigit({})",  "uc_is_digit({})",  "uc_is_digit({})", ],
        "TISXDIGIT": ["isxdigit((unsigned char){})", "iswxdigit({})", "uc_is_xdigit({})", "uc_is_xdigit({})", ],
        "TSTRCHR":   ["strchr({})",                  "wcschr({})",    "u16_strchr({})",   "u32_strchr({})", ],
        "TSTRCMP":   ["strcmp({})",                  "wcscmp({})",    "u16_strcmp({})",   "u32_strcmp({})", ],
        "TSTRLEN":   ["strlen({})",                  "wcslen({})",    "u16_strlen({})",   "u32_strlen({})", ],
    },
}
# fmt: on


@dataclass
class Float:
    # N - "name"
    N: str
    # G - "group". s = "standard", "f" = floats, "d" = decimals
    G: str
    type: str
    math: str
    strto: str


j_FLOATS = [
    Float("f", "s", "float", "f", "f"),
    Float("d", "s", "double", "", "d"),
    Float("l", "s", "long double", "l", "ld"),
    Float("f16", "f", "_Float16", "f16", "f16"),
    Float("f32", "f", "_Float32", "f32", "f32"),
    Float("f64", "f", "_Float64", "f64", "f64"),
    Float("f128", "f", "_Float128", "f128", "f128"),
    Float("f32x", "fx", "_Float32x", "f32x", "f32x"),
    Float("f64x", "fx", "_Float64x", "f64x", "f64x"),
    Float("f128x", "fx", "_Float128x", "f128x", "f128x"),
    Float("d32", "d", "_Decimal32", "d32", "d32"),
    Float("d64", "d", "_Decimal64", "d64", "d64"),
    Float("d128", "d", "_Decimal128", "d128", "d128"),
    Float("d32x", "dx", "_Decimal32x", "d32x", "d32x"),
    Float("d64x", "dx", "_Decimal64x", "d64x", "d64x"),
    Float("d128x", "dx", "_Decimal128x", "d128x", "d128x"),
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
    return bool(re.match(regex, str(value)))


def j_fatal(value="fatal error"):
    raise Exception(str(value))


def j_frametemplate():
    template = None
    for frameInfo in inspect.stack():
        if frameInfo.frame.f_globals.get("__jinja_template__") is not None:
            template = frameInfo.frame.f_globals.get("__jinja_template__")
            break
    return template


def j_lineno():
<<<<<<< Updated upstream
    ft = j_frametemplate()
    assert ft
    curf = inspect.currentframe()
    assert curf
    assert curf.f_back
    return ft.get_corresponding_lineno(curf.f_back.f_lineno)
||||||| Stash base
    import inspect

    return j_frametemplate().get_corresponding_lineno(
        inspect.currentframe().f_back.f_lineno
    )
=======
    ft = j_frametemplate()
    assert ft
    cf = inspect.currentframe()
    assert cf
    cfback = cf.f_back
    assert cfback
    return ft.get_corresponding_lineno(cfback.f_lineno)
>>>>>>> Stashed changes


def test_integer(value) -> bool:
    """Return true if the object is an integer.
    .. versionadded:: 2.11
    """
    return isinstance(value, int) and value is not True and value is not False


IMPORTS_FROM_LIBRARY_JINJA = ",".join(
    [
        "j_seq",
        "j_seqcomma",
        "j_seqdashcomma",
        "j_APPLY_IN",
        "j_APPLY",
        "j_FOREACHAPPLY",
        "j_FUNC",
        "j_yio_macros_funcs",
        "j_yio_macros_args",
        "j_yio_macros_funcs",
        "j_STDFIX",
    ]
)

PREFIX = "{% from 'library.jinja' import " + IMPORTS_FROM_LIBRARY_JINJA + " %}"

###############################################################################

<<<<<<< Updated upstream
||||||| Stash base
DEPENDENCIES = set()


class MFSLoader(jinja2.FileSystemLoader):
    """
    A normal loader, just stores referenced tepmlates in dependencies
    https://gist.github.com/Zoramite/f4c42620d7b564a26a398d8d25ecb419
    """

    def get_source(self, environment, template):
        source, filename, uptodate = super(MFSLoader, self).get_source(
            environment, template
        )
        global DEPENDENCIES
        DEPENDENCIES.add(filename)
        return source, filename, uptodate


def shoulddoline(source):
    global DEBUG
    return (DEBUG and not re.match("NOLINE", source))


class SuperPreprocess(jinja2.ext.Extension):
    """
    Custom plugin for preprocessing source files according to custom rules
    basically signifiicantly extending jinja2
    """
=======
DEPENDENCIES: Set[str] = set()


class MFSLoader(jinja2.FileSystemLoader):
    """
    A normal loader, just stores referenced tepmlates in dependencies
    https://gist.github.com/Zoramite/f4c42620d7b564a26a398d8d25ecb419
    """

    def get_source(self, environment, template):
        source, filename, uptodate = super(MFSLoader, self).get_source(
            environment, template
        )
        global DEPENDENCIES
        DEPENDENCIES.add(filename)
        return source, filename, uptodate


def shoulddoline(source):
    global DEBUG
    return DEBUG and not re.match("NOLINE", source)


class SuperPreprocess(jinja2.ext.Extension):
    """
    Custom plugin for preprocessing source files according to custom rules
    basically signifiicantly extending jinja2
    """
>>>>>>> Stashed changes

class MyPreprocess(jinja2.ext.Extension):
    def preprocess(self, source, name, filename=None):
        output = PREFIX
        for lineno, line in enumerate(source.split("\n")):
            if line == "#line":
                # Replace '#line' by the proper C directive.
                line = '#line {lineno + 2} "{filename}"'
            output += line + "\n"
        return output


def postprocess(output, infilename, mode):
    global TDATA
    log.debug("mode=" + str(mode) + "\t" + infilename)
    tmpl = None
    if mode is not None and mode != "none":
        tmpl = TDATA[template_data["mode"].index(mode)]
        # Replace Ω and π.
        output = output.replace("Ω", tmpl["omega"]).replace("π", tmpl["pi"])
        # Replace names.
        for kk, vv in tmpl["names"]:
            output = re.sub(r"\b{}\b".format(kk), vv, output)
        # Replace function calls
        for kk, vv in tmpl["funcs"]:
            output = re.sub(
                r"\b{}\b\s*\(([^\)]*)\)".format(kk),
                vv.replace("{}", r"\1"),
                output,
            )
        # Replace YYIO_TYPE_Y*IO by 1
        output = re.sub("YYIO_TYPE_" + mode.upper(), "1", output)
    header = f"""\
// Do not edit this file! This file was auto-generated by preprocess.py at {datetime.datetime.now()}
// To change this file, edit the source.
// mode = {mode}
#line 1 "{infilename}"
"""
    output = header + output
    return output


def save_if_changed(output, outfilename, msg):
    """Output output to outfilename only if changed. Print msg for logs"""
    global log
    if os.path.exists(outfilename):
        if open(outfilename, "r").read() == output:
            # LL.debug("NOCHANGE: " + msg)
            return
        os.chmod(outfilename, 0o644)
    else:
        os.makedirs(os.path.dirname(os.path.realpath(outfilename)), exist_ok=True)
    with open(outfilename, "w") as outfile:
        os.chmod(outfilename, 0o444)
        log.debug("GENERATED: " + msg)
        outfile.write(output)


def invert_template_data():
    """Invert template_data"""
    global TDATA, template_data
    # Add /*TMODE*/ and /*TMODEX*/ to template_data
    for k in ["TMODE", "TMODEX"]:
        template_data["names"][k] = [
            str(i) + "/*" + k + "*/" for i in template_data["names"][k]
        ]
    TDATA = []
    for i in range(4):
        tmp = {}
        for k in template_data.keys():
            if isinstance(template_data[k], list):
                tmp[k] = str(template_data[k][i])
            else:
                tmp[k] = []
                for k2 in template_data[k]:
                    tmp[k] += [(k2, str(template_data[k][k2][i]))]
        TDATA += [tmp]


<<<<<<< Updated upstream
||||||| Stash base
def find_dependencies():
    """Find all files with .jinja suffix and add them as dependencies"""
    global DEPENDENCIES
    DEPENDENCIES = [__file__]
    for (dirpath, _, filenames) in os.walk(SRCDIR):
        for ff in filenames:
            if ff.endswith(".jinja"):
                DEPENDENCIES += [os.path.join(dirpath, ff)]


=======
def find_dependencies():
    """Find all files with .jinja suffix and add them as dependencies"""
    global DEPENDENCIES
    DEPENDENCIES = [__file__]
    for dirpath, _, filenames in os.walk(SRCDIR):
        for ff in filenames:
            if ff.endswith(".jinja"):
                DEPENDENCIES += [os.path.join(dirpath, ff)]


>>>>>>> Stashed changes
def parse_arguments():
    # Parse arguments
    parser = argparse.ArgumentParser(description="")
    parser.add_argument("-S", "--srcdir", default=[DIR], action="append")
    parser.add_argument("-D", "--define", action="append", default=[])
    parser.add_argument("-C", "--cachedir")
    parser.add_argument("-T", "--depfile")
    parser.add_argument(
        "-m", "--mode", default="none", choices=(["none"] + template_data["mode"])
    )
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument("-d", "--debug", action="store_true")
    parser.add_argument("source")
    parser.add_argument("output")
    args = parser.parse_args()
    logging.basicConfig(
        format="%(funcName)s:%(lineno)s:\t%(message)s",
        level=logging.DEBUG if args.verbose else logging.INFO,
    )
    SRCDIR = args.srcdir
    return args


def prepare_environment(args):
    """Setup jinja2 environment"""
    if args.cachedir is not None:
        os.makedirs(args.cachedir, exist_ok=True)
    env = jinja2.Environment(
        loader=jinja2.FileSystemLoader(SRCDIR),
        extensions=[
            MyPreprocess,
        ],
        trim_blocks=True,
        lstrip_blocks=True,
        cache_size=0,
        bytecode_cache=(
            jinja2.FileSystemBytecodeCache(args.cachedir)
            if args.cachedir is not None
            else None
        ),
        undefined=jinja2.StrictUndefined,
    )
    defglobals = {
        "j_MLVLS": 5,
        "j_SLOTS": 5,
        "j_FLOATS": j_FLOATS,
        "j_range": j_range,
        "j_match": j_match,
        "j_fatal": j_fatal,
        "j_lineno": j_lineno,
    }
    for ii in args.define:
        kk, vv = ii.split("=", 2)
        defglobals[kk] = vv
    env.globals.update(defglobals)
    env.tests["integer"] = test_integer
    return env


if __name__ == "__main__":
    args = parse_arguments()
    invert_template_data()
    env = prepare_environment(args)
    mode = args.mode
    ttemplate = env.get_template(args.source)
    infilename = ttemplate.filename
    output = ttemplate.render()
    output = postprocess(output, infilename, mode)
    outfilename = args.output
    assert infilename
    save_if_changed(output, outfilename, infilename + "\t->\t" + outfilename)

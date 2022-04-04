#!/usr/bin/env python3

import argparse
import jinja2
import jinja2.ext
import logging
import os
import re
import sys

###############################################################################

# fmt: off
template_data = {
        "mode":  ["yio", "ywio", "yc16io", "yuio", ],
        "omega": ["",    "W",    "C16",    "U", ],
        "pi":    ["",    "w",    "c16",    "u", ],
        "names": {
            "Ychar": ["char",  "wchar_t", "uint16_t",   "uint32_t", ],
            "Yint":  ["int",   "wint_t",  "uint16_t",   "uint32_t", ],
            "YEOF":  ["EOF",   "WEOF",    "UINT16_MAX", "UINT32_MAX", ],
            "YPRI":  ["\"s\"", "\"ls\"",  "\"lU\"",     "\"llU\"", ],
            },
        "funcs": {
            "Yc":       ["{}",                         "L{}",          "u{}",                    "U{}", ],
            "Yfprintf": ["fprintf({})",                "fprintf({})",  "ulc_fprintf({})",        "ulc_fprintf({})", ],
            "Yisdigit": ["isdigit((unsigned char){})", "iswdigit({})", "uc_is_digit({})",        "uc_is_digit({})", ],
            "Yisprint": ["isprint((unsigned char){})", "iswprint({})", "uc_is_print({})",        "uc_is_print({})", ],
            "Yisupper": ["isupper((unsigned char){})", "iswupper({})", "uc_is_upper({})",        "uc_is_upper({})", ],
            "Yprintf":  ["printf({})",                 "printf({})",   "ulc_fprintf(stdout,{})", "ulc_fprintf(stdout,{})", ], # noqa
            "Ystrchr":  ["strchr({})",                 "wcschr({})",   "u16_strchr({})",         "u32_strchr({})", ],
            "Ystrcmp":  ["strcmp({})",                 "wcscmp({})",   "u16_strcmp({})",         "u32_strcmp({})", ],
            "Ystrlen":  ["strlen({})",                 "wcslen({})",   "u16_strlen({})",         "u32_strlen({})", ],
            },
        }
# fmt: on

###############################################################################


def isnewer(a, b):
    return os.path.getmtime(a) > os.path.getmtime(b)


def isnewerall(f, *args):
    for i in args:
        if not isnewer(f, i):
            return False
    return True


class SuperPreprocess(jinja2.ext.Extension):
    """
    Custom plugin for preprocessing source files according to custom rules
    basically signifiicantly extending jinja2
    """

    def preprocess(self, source, name, filename=None):
        global TMPL, DEBUG
        assert filename is not None
        #
        LL.debug(TMPL["mode"] + "\t" + name)
        # Replace Ω and π.
        source = source.replace("Ω", TMPL["omega"]).replace("π", TMPL["pi"])
        # Replace names.
        for kk, vv in TMPL["names"]:
            source = re.sub(r"\b%s\b" % kk, vv, source)
        # Call functions.
        for kk, vv in TMPL["funcs"]:
            source = re.sub(
                r"\b%s\b\s*\(([^\)]*)\)" % kk, vv.replace("{}", r"\1"), source
            )
        # Replace '#line' by the proper C directive.
        output = ""
        if DEBUG:
            output += f"#line 1 {filename}" + "\n"
        for lineno, line in enumerate(source.split("\n")):
            if "#line" in line:
                if not DEBUG:
                    if not line == "#line":
                        line = line.replace("#line", "")
                        output += line + "\n"
                else:
                    line = line.replace("#line", f"#line {lineno} {filename}")
                    output += line + "\n"
            else:
                output += line + "\n"
        return output


def do_template(env, infilename, reloutfilename, mode=None):
    """Actually run the templating."""
    global TMPL, template_data, OUTDIR, INDIR, DEPENDENCIES
    TMPL = TDATA[0 if mode is None else template_data["mode"].index(mode)]
    #
    relinfilename = os.path.relpath(infilename, INDIR)
    outfilename = os.path.join(OUTDIR, reloutfilename)
    exists = os.path.exists(outfilename)
    if exists:
        if isnewerall(outfilename, infilename, *DEPENDENCIES):
            # LL.debug("ISNEWERANY\t" + relinfilename + "\t->\t" + reloutfilename)
            return
    res = env.get_template(relinfilename).render()
    if exists:
        os.chmod(outfilename, 0o666)
    os.makedirs(os.path.dirname(outfilename), exist_ok=True)
    outfile = open(outfilename, "w+")
    os.chmod(outfilename, 0o444)
    LL.debug(relinfilename + "\t->\t" + reloutfilename)
    if outfile.read() != res:
        outfile.seek(0)
        print(res, file=outfile)


###############################################################################

# Some globals assignment
logging.basicConfig(format="%(funcName)s:%(lineno)s:\t%(message)s")
LL = logging.getLogger(os.path.basename(__file__))
LL.setLevel(logging.DEBUG)
DIR = os.path.dirname(__file__)

# Parse arguments
parser = argparse.ArgumentParser(description="")
parser.add_argument("-I", "--indir", default=DIR)
parser.add_argument("-O", "--outdir", required=True)
parser.add_argument("-D", "--define", action="append", default=[])
parser.add_argument("-v", "--verbose", action="store_true")
parser.add_argument("-d", "--debug", action="store_true")
parser.add_argument("--test", action="store_true")
args = parser.parse_args()

# Some globals assgnment
LL.setLevel("DEBUG" if args.verbose else "INFO")
INDIR = args.indir
OUTDIR = args.outdir
DEBUG = args.debug

# Setup jinja2 environment
env = jinja2.Environment(
    loader=jinja2.FileSystemLoader(INDIR),
    extensions=[
        SuperPreprocess,
    ],
    trim_blocks=True,
    lstrip_blocks=True,
)
env.globals["j_MLVLS"] = args.MLVLS
for ii in args.define:
    kk, vv = ii.split("=", 2)
    env.globals[kk] = vv

# invert template data, so that mode index is the first dimension.
TDATA = [{}] * 4
for i in range(4):
    TDATA[i] = {}
    for k in template_data.keys():
        if isinstance(template_data[k], list):
            TDATA[i][k] = template_data[k][i]
        else:
            TDATA[i][k] = []
            for k2 in template_data[k]:
                TDATA[i][k] += [(k2, template_data[k][k2][i])]
TMPL = TDATA[0]

if args.test:
    do_template(env, "yio/template/io.c", "yio/ywio/io.c", "ywio")
    sys.exit(0)

# Find all files with .jinja suffix and add them as dependencies
DEPENDENCIES = [__file__]
for (dirpath, _, filenames) in os.walk(INDIR):
    for ff in filenames:
        if ff.endswith(".jinja"):
            DEPENDENCIES += [os.path.join(dirpath, ff)]

# Preprocess all files from INDIR
for (dirpath, _, filenames) in os.walk(INDIR):
    relpath = os.path.relpath(dirpath, INDIR)
    for ff in filenames:
        # Tempalte only .h and .c files
        if not ff.endswith(".h") and not ff.endswith(".c"):
            continue
        infilename = os.path.join(dirpath, ff)
        # Handle templates folder 4 times separately
        if relpath.startswith("yio/template/"):
            for mode in ["yio", "ywio", "yuio", "yc16io"]:
                do_template(
                    env,
                    infilename,
                    os.path.join(
                        "yio",
                        mode,
                        os.path.relpath(
                            dirpath, os.path.join(INDIR, "yio", "template")
                        ),
                        ff,
                    ),
                    mode,
                )
            continue
        # Handle rest .c and .h files
        do_template(env, infilename, os.path.join(relpath, ff))

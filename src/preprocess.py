#!/usr/bin/env python3

import argparse
import jinja2
import jinja2.ext
import jinja2.meta
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
    ["d64x", "dx",  "_Decimal64x", "d64x", "d64x"],
    ["d128x", "dx", "_Decimal128x", "d128x", "d128x"],
]

j_FLOATS = [
    # "invert" the table and replace N for 0, so it is j_FLOATS = [ {0:"f", "type": "float", etc..}, ... ]
    (dict([(0, x[0])]) | {v: k for v, k in zip(j_FLOATS[0], x) if v != "N"})
    for x in j_FLOATS[1:]
]

###############################################################################


def isnewer(a, b):
    return os.path.getmtime(a) > os.path.getmtime(b)


def isnewerall(f, *args):
    for i in args:
        if not isnewer(f, i):
            return False
    return True


DEPENDENCIES = set()


class MFSLoader(jinja2.FileSystemLoader):
    """
    A normal loader, just stores referenced tepmlates in dependencies
    https://gist.github.com/Zoramite/f4c42620d7b564a26a398d8d25ecb419
    """

    dependencies = {}

    def get_source(self, environment, template):
        source, filename, uptodate = super(MFSLoader, self).get_source(
            environment, template
        )
        global DEPENDENCIES
        DEPENDENCIES.add(filename)
        return source, filename, uptodate


class SuperPreprocess(jinja2.ext.Extension):
    """
    Custom plugin for preprocessing source files according to custom rules
    basically signifiicantly extending jinja2
    """

    def preprocess(self, source, name, filename=None):
        global DEBUG
        #
        output = ""
        # Add autoincludes
        if name is not None and not name.endswith(".jinja"):
            imports = [
                "j_seq",
                "j_seqcomma",
                "j_seqdashcomma",
                "j_APPLY_IN",
                "j_APPLY",
                "j_FOREACHAPPLY",
                "j_FUNC",
            ]
            output += "{% from 'library.jinja' import " + ",".join(imports) + " %}"
        # Replace '#line' by the proper C directive.
        for lineno, line in enumerate(source.split("\n")):
            if "#line" in line:
                line = line.replace(
                    "#line",
                    '#line {} "{}"'.format(lineno + 2, filename) if DEBUG else "",
                )
                output += line + "\n"
            else:
                output += line + "\n"
        return output


def postprocess(output, infilename, mode):
    global DEBUG, TDATA
    LL.debug("mode=" + str(mode) + "\t" + infilename)
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
        # Replace _yIO_TYPE_Y*IO by 1
        output = re.sub("_yIO_TYPE_" + mode.upper(), "1", output)
    if DEBUG:
        header = """
#if 0
// Do not edit this file! This file was auto-generated by preprocess.py
// To change this file, edit the source.
// mode = %s
#endif
#line 1 "%s"
""" % (
            mode,
            infilename,
        )
        output = header + output
    return output


def save_if_changed(output, infilename, outfilename):
    if os.path.exists(outfilename):
        if open(outfilename, "r").read() == output:
            LL.debug("NOCHANGE: " + infilename + "\t->\t" + outfilename)
            return
        os.chmod(outfilename, 0o644)
    else:
        os.makedirs(os.path.dirname(os.path.realpath(outfilename)), exist_ok=True)
    with open(outfilename, "w") as outfile:
        os.chmod(outfilename, 0o444)
        LL.debug(infilename + "\t->\t" + outfilename)
        outfile.write(output)


def do_template(env, infilename, reloutfilename, mode=None):
    """Actually run the templating."""
    global GENDIR, SRCDIR, DEPENDENCIES, MODE
    #
    relinfilename = os.path.relpath(infilename, SRCDIR)
    outfilename = os.path.join(GENDIR, reloutfilename)
    exists = os.path.exists(outfilename)
    # if exists:
    # if isnewerall(outfilename, infilename, *DEPENDENCIES):
    # LL.debug("ISNEWERANY\t" + relinfilename + "\t->\t" + reloutfilename)
    # return
    #
    template = env.get_template(relinfilename)
    env.find_referenced_templates(template)
    output = template.render(
        {
            "MODE": mode,
            "MODEX": dict({"yio": 1, "ywio": 2}).get(mode, 3),
        }
    )
    output = postprocess(output, infilename, mode)
    #
    save_if_changed(output, exists, outfilename, relinfilename, reloutfilename)


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


def j_fatal(value):
    raise Exception(value)


def j_frametemplate():
    import inspect
    template = None
    for frameInfo in inspect.stack():
        if frameInfo.frame.f_globals.get("__jinja_template__") is not None:
            template = frameInfo.frame.f_globals.get("__jinja_template__")
            break
    return template


def j_lineno():
    return j_frametemplate().get_corresponding_lineno(
        inspect.currentframe().f_back.f_lineno
    )


def find_files(path, extensions, filter):
    ret = []
    for (dirpath, _, filenames) in os.walk(path):
        for filename in filenames:
            if filename.exndwith(extensions):
                if filter(filename):
                    ret += [os.path.join(dirpath, filename)]
    return ret


def invert_template_data():
    # Invert template_data
    global TDATA, template_data
    TDATA = []
    for i in range(4):
        tmp = {}
        for k in template_data.keys():
            if isinstance(template_data[k], list):
                tmp[k] = template_data[k][i]
            else:
                tmp[k] = []
                for k2 in template_data[k]:
                    tmp[k] += [(k2, template_data[k][k2][i])]
        TDATA += [tmp]


def handle_test_arg(args):
    if args.test:
        do_template(env, "yio/template/io.c", "yio/ywio/io.c", "ywio")
        sys.exit(0)


def find_dependencies():
    """Find all files with .jinja suffix and add them as dependencies"""
    global DEPENDENCIES
    DEPENDENCIES = [__file__]
    for (dirpath, _, filenames) in os.walk(SRCDIR):
        for ff in filenames:
            if ff.endswith(".jinja"):
                DEPENDENCIES += [os.path.join(dirpath, ff)]


def process_all_files_in_srcdir():
    # Preprocess all files from SRCDIR
    for (dirpath, _, filenames) in os.walk(SRCDIR):
        relpath = os.path.relpath(dirpath, SRCDIR)
        for ff in filenames:
            # Template only .h and .c files
            if not ff.endswith(".h") and not ff.endswith(".c"):
                continue
            infilename = os.path.join(dirpath, ff)
            # Handle templates folder 4 times separately
            if relpath.startswith("yio/template"):
                for mode in ["yio", "ywio", "yuio", "yc16io"]:
                    do_template(
                        env,
                        infilename,
                        os.path.join(
                            "yio",
                            mode,
                            os.path.relpath(
                                dirpath, os.path.join(SRCDIR, "yio", "template")
                            ),
                            ff,
                        ),
                        mode,
                    )
                continue
            # Handle rest .c and .h files
            do_template(env, infilename, os.path.join(relpath, ff))


def parse_arguments():
    global LL, DIR, SRCDIR, DEBUG
    # Some globals assignment
    logging.basicConfig(format="%(funcName)s:%(lineno)s:\t%(message)s")
    LL = logging.getLogger(os.path.basename(__file__))
    LL.setLevel(logging.DEBUG)
    DIR = os.path.dirname(__file__)

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
    parser.add_argument("--test", action="store_true")
    parser.add_argument("source")
    parser.add_argument("output")
    args = parser.parse_args()

    # Some globals assgnment
    LL.setLevel("DEBUG" if args.verbose else "INFO")
    SRCDIR = args.srcdir
    DEBUG = args.debug

    return args


def prepare_environment(args):
    """Setup jinja2 environment"""
    if args.cachedir is not None:
        os.makedirs(args.cachedir, exist_ok=True)
    env = jinja2.Environment(
        loader=(jinja2.FileSystemLoader if args.depfile is None else MFSLoader)(SRCDIR),
        extensions=[
            SuperPreprocess,
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
        "j_SLOTS_END": 105,
        "j_range": j_range,
        "j_match": j_match,
        "fatal": j_fatal,
        "j_FLOATS": j_FLOATS,
        "j_lineno": j_lineno,
    }
    for ii in args.define:
        kk, vv = ii.split("=", 2)
        defglobals[kk] = vv
    env.globals.update(defglobals)
    return env


def depfile_path(path):
    return os.path.realpath(path).replace(" ", "\\ ")


def generate_depfile(depfile, env, infilename, outfilename):
    if depfile is None:
        return
    LL.debug("Writing depfile " + depfile)
    os.makedirs(os.path.dirname(depfile), exist_ok=True)
    print(
        depfile_path(outfilename)
        + ":"
        + "".join([" " + depfile_path(dd) for dd in DEPENDENCIES]),
        file=open(depfile, "w"),
    )


args = parse_arguments()
invert_template_data()
env = prepare_environment(args)
handle_test_arg(args)

mode = args.mode
ttemplate = env.get_template(args.source)
infilename = ttemplate.filename
output = ttemplate.render(
    {
        "MODE": mode.upper(),
        "MODEX": dict({"none": -1, "yio": 1, "ywio": 2}).get(mode, 3),
    }
)
output = postprocess(output, infilename, mode)
outfilename = args.output
save_if_changed(output, infilename, outfilename)
generate_depfile(args.depfile, env, infilename, outfilename)

#!/usr/bin/env python3
import jinja2
import os
import argparse


def eq_to_dict(strs):
    return dict(arg.split("=") for arg in strs)


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--searchpath", action="append", default=["/"])
parser.add_argument("-e", "--environment", action="append", default=[])
parser.add_argument("-r", "--render", action="append", default=[])
parser.add_argument("inputfile")
parser.add_argument("outputfile")
args = parser.parse_args()

templateLoader = jinja2.FileSystemLoader(searchpath=args.searchpath)
envOpts = eq_to_dict(args.environment)
templateEnv = jinja2.Environment(loader=templateLoader, **envOpts)
template = templateEnv.get_template(args.inputfile)
outputText = template.render(**eq_to_dict(args.render))
os.makedirs(os.path.dirname(args.outputfile), exist_ok=True)
print(outputText, file=open(args.outputfile, "w"))

#!/usr/bin/env python3
# /// script
# dependencies = []
# ///
import argparse
import re
import sys


class FStringError(Exception):
    """Custom exception for f-string parsing errors."""

    def __init__(self, message, line=None):
        self.message = message
        self.line = line
        super().__init__(self.message)


def find_format_separator(text):
    """
    Scans a C expression to find the boundary colon between the expression and the format specifier.
    Correctly handles nested (), [], {}, strings, char literals, and ternary operators.
    """
    i = 0
    bracket_stack = []
    question_count = 0

    while i < len(text):
        char = text[i]

        # Handle strings
        if char == '"':
            i += 1
            while i < len(text) and text[i] != '"':
                if text[i] == "\\":
                    i += 1
                i += 1
            if i >= len(text):
                raise FStringError("Unterminated string literal in expression")
            i += 1
            continue

        # Handle char literals
        if char == "'":
            i += 1
            while i < len(text) and text[i] != "'":
                if text[i] == "\\":
                    i += 1
                i += 1
            if i >= len(text):
                raise FStringError("Unterminated character literal in expression")
            i += 1
            continue

        # Handle nesting
        if char in "({[":
            bracket_stack.append(char)
        elif char in ")}]":
            if not bracket_stack:
                raise FStringError(f"Unmatched closing bracket '{char}'")
            opening = bracket_stack.pop()
            if (
                (opening == "(" and char != ")")
                or (opening == "{" and char != "}")
                or (opening == "[" and char != "]")
            ):
                raise FStringError(f"Mismatched brackets: '{opening}' and '{char}'")

        # Only check for ? and : at top level
        if not bracket_stack:
            if char == "?":
                question_count += 1
            elif char == ":":
                if question_count > 0:
                    question_count -= 1
                else:
                    return i

        i += 1

    if bracket_stack:
        raise FStringError(f"Unmatched opening bracket '{bracket_stack[-1]}'")
    if question_count > 0:
        raise FStringError("Unterminated ternary operator (missing ':')")

    return -1


def process_fstring_content(content):
    args, parts, i = [], [], 0
    while i < len(content):
        # Handle escaped braces
        if content[i : i + 2] == "{{":
            parts.append("{{")
            i += 2
            continue
        if content[i : i + 2] == "}}":
            parts.append("}}")
            i += 2
            continue

        if content[i] == "{":
            start = i
            count = 1
            j = i + 1
            while j < len(content) and count > 0:
                if content[j] == '"':
                    j += 1
                    while j < len(content) and content[j] != '"':
                        if content[j] == "\\":
                            j += 2
                        else:
                            j += 1
                    if j >= len(content):
                        raise FStringError("Unterminated string in f-string expression")
                    j += 1
                elif content[j] == "'":
                    j += 1
                    while j < len(content) and content[j] != "'":
                        if content[j] == "\\":
                            j += 2
                        else:
                            j += 1
                    if j >= len(content):
                        raise FStringError("Unterminated character literal in f-string expression")
                    j += 1
                elif content[j] == "{":
                    count += 1
                    j += 1
                elif content[j] == "}":
                    count -= 1
                    j += 1
                else:
                    j += 1

            if count == 0:
                expr_full = content[start + 1 : j - 1]

                def process_nested_fmt(text):
                    if "{" not in text:
                        return text, []
                    res_parts, res_args, k = [], [], 0
                    while k < len(text):
                        if text[k : k + 2] == "{{":
                            res_parts.append("{{")
                            k += 2
                        elif text[k : k + 2] == "}}":
                            res_parts.append("}}")
                            k += 2
                        elif text[k] == "{":
                            s, c, m = k, 1, k + 1
                            while m < len(text) and c > 0:
                                if text[m] == "{":
                                    c += 1
                                elif text[m] == "}":
                                    c -= 1
                                m += 1
                            if c == 0:
                                res_args.append(text[s + 1 : m - 1].strip())
                                res_parts.append("{}")
                                k = m
                            else:
                                raise FStringError("Unmatched brace in nested format specifier")
                        else:
                            res_parts.append(text[k])
                            k += 1
                    return "".join(res_parts), res_args

                if expr_full.endswith("="):
                    var_name = expr_full[:-1].strip()
                    args.append(var_name)
                    parts.append(f"{var_name}={{}}")
                elif "=:" in expr_full:
                    var_name, fmt = expr_full.split("=:", 1)
                    fmt_str, fmt_args = process_nested_fmt(fmt)
                    args.append(var_name.strip())
                    args.extend(fmt_args)
                    parts.append(f"{var_name.strip()}={{:{fmt_str}}}")
                else:
                    sep_pos = find_format_separator(expr_full)
                    if sep_pos != -1:
                        expr, fmt = expr_full[:sep_pos], expr_full[sep_pos + 1 :]
                        fmt_str, fmt_args = process_nested_fmt(fmt)
                        args.append(expr.strip())
                        args.extend(fmt_args)
                        parts.append(f"{{:{fmt_str}}}")
                    else:
                        args.append(expr_full.strip())
                        parts.append("{}")
                i = j
            else:
                raise FStringError("Unmatched '{' in f-string")
        else:
            parts.append(content[i])
            i += 1
    return "".join(parts), args


def unescape_c_string(s):
    """Unescape C string content (handles at least \" and \\)."""
    res = []
    i = 0
    while i < len(s):
        if s[i] == "\\" and i + 1 < len(s):
            if s[i + 1] == '"':
                res.append('"')
                i += 2
            elif s[i + 1] == "'":
                res.append("'")
                i += 2
            elif s[i + 1] == "\\":
                res.append("\\")
                i += 2
            else:
                res.append(s[i])
                i += 1
        else:
            res.append(s[i])
            i += 1
    return "".join(res)


def tokenize(content):
    tokens, i = [], 0
    while i < len(content):
        start = i
        if i + 1 < len(content) and content[i].lower() == "f" and content[i + 1] == '"':
            i += 2
        elif content[i] == '"':
            i += 1
        else:
            m = re.match(r'([^fF"]+|[fF](?!")|"(?!))', content[i:])
            val = m.group(1) if m else content[i]
            tokens.append({"type": "OTHER", "val": val, "line": content[:start].count("\n") + 1})
            i += len(val)
            continue

        while i < len(content) and content[i] != '"':
            if content[i] == "\\":
                i += 2
            else:
                i += 1

        if i >= len(content):
            raise FStringError("Unterminated string literal", line=content[:start].count("\n") + 1)
        i += 1
        val = content[start:i]
        tokens.append({"type": "STR", "val": val, "line": content[:start].count("\n") + 1})

    return tokens


def escape_c_string(s):
    """Escapes a string for use in a C string literal."""
    return s.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n")


def preprocess(content, filename="<stdin>"):
    try:
        tokens = tokenize(content)
    except FStringError as e:
        sys.stderr.write(f"{filename}:{e.line}: error: {e.message}\n")
        sys.exit(1)

    merged, i = [], 0
    while i < len(tokens):
        if tokens[i]["type"] == "STR":
            group, j = [tokens[i]], i + 1
            last_str_idx = 0
            while j < len(tokens):
                if tokens[j]["type"] == "OTHER" and tokens[j]["val"].isspace():
                    group.append(tokens[j])
                    j += 1
                elif tokens[j]["type"] == "STR":
                    group.append(tokens[j])
                    last_str_idx = len(group) - 1
                    j += 1
                else:
                    break

            group = group[: last_str_idx + 1]
            j = i + len(group)

            has_f = any(t["val"].lower().startswith("f") for t in group)
            if has_f:
                all_fmt, all_args, first_line = [], [], group[0]["line"]
                for t in group:
                    if t["type"] == "STR":
                        val, is_f = t["val"], t["val"].lower().startswith("f")
                        raw_content = val[2:-1] if is_f else val[1:-1]
                        inner = unescape_c_string(raw_content)
                        if is_f:
                            try:
                                fmt, args = process_fstring_content(inner)
                                all_fmt.append(fmt)
                                all_args.extend(args)
                            except FStringError as e:
                                line = t["line"] + (e.line or 0)
                                sys.stderr.write(f"{filename}:{line}: error: {e.message}\n")
                                sys.exit(1)
                        else:
                            all_fmt.append(inner.replace("{", "{{").replace("}", "}}"))
                    else:
                        # Whitespace between strings is ignored in the format string
                        pass

                replacement = f'"{escape_c_string("".join(all_fmt))}"'
                if all_args:
                    replacement += f", {', '.join(all_args)}"

                last_line = group[-1]["line"] + group[-1]["val"].count("\n")
                merged.append({"type": "OTHER", "val": replacement})

                if len([t for t in group if t["type"] == "STR"]) > 1 and last_line > first_line:
                    merged.append({"type": "OTHER", "val": f'\n#line {last_line} "{filename}"\n'})
                i = j
            else:
                merged.append(tokens[i])
                i += 1
        else:
            merged.append(tokens[i])
            i += 1
    return "".join(t["val"] for t in merged)


def main():
    parser = argparse.ArgumentParser(
        description="Python-based f-string preprocessor for C source code."
    )
    parser.add_argument("input", nargs="?", help="Input C file (use '-' for stdin)")
    args = parser.parse_args()

    if args.input is None:
        parser.print_help()
        sys.exit(0)

    try:
        if args.input == "-":
            sys.stdout.write(preprocess(sys.stdin.read(), "<stdin>"))
        else:
            with open(args.input) as f:
                sys.stdout.write(preprocess(f.read(), args.input))
    except Exception as e:
        sys.stderr.write(f"error: {e}\n")
        sys.exit(1)


if __name__ == "__main__":
    main()

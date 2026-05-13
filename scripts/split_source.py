#!/usr/bin/env python3
import argparse
import os
import re
import sys

def split_source(input_path, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    with open(input_path, 'r') as f:
        content = f.read()

    lines = content.splitlines(keepends=True)
    
    preamble = []
    functions = []
    
    # Improved regex to find function starts:
    # 1. Start of line (no leading whitespace to ensure top-level)
    # 2. Return type (at least one char, not starting with whitespace or #)
    # 3. Function name
    # 4. Parentheses with arguments
    # 5. Optional opening brace
    func_start_re = re.compile(r'^([^#\s][\w\s\*]*?)\s+(\w+)\s*\(([^;]*?)\)\s*\{?', re.MULTILINE | re.DOTALL)
    
    keywords = {'if', 'while', 'for', 'switch', 'return', 'else', 'do', 'typedef', 'struct', 'union', 'enum'}
    
    idx = 0
    first_func_start = len(content)
    
    # State machine for brace counting and ignoring strings/comments
    def find_closing_brace(start_pos):
        depth = 0
        i = start_pos
        in_string = False
        in_char = False
        in_line_comment = False
        in_block_comment = False
        escaped = False
        
        while i < len(content):
            char = content[i]
            next_char = content[i+1] if i + 1 < len(content) else ''
            
            if in_line_comment:
                if char == '\n':
                    in_line_comment = False
            elif in_block_comment:
                if char == '*' and next_char == '/':
                    in_block_comment = False
                    i += 1
            elif in_string:
                if escaped:
                    escaped = False
                elif char == '\\':
                    escaped = True
                elif char == '"':
                    in_string = False
            elif in_char:
                if escaped:
                    escaped = False
                elif char == '\\':
                    escaped = True
                elif char == "'":
                    in_char = False
            else:
                if char == '/' and next_char == '/':
                    in_line_comment = True
                    i += 1
                elif char == '/' and next_char == '*':
                    in_block_comment = True
                    i += 1
                elif char == '"':
                    in_string = True
                elif char == "'":
                    in_char = True
                elif char == '{':
                    depth += 1
                elif char == '}':
                    depth -= 1
                    if depth == 0:
                        return i
            i += 1
        return -1

    # Find all function starts
    matches = list(func_start_re.finditer(content))
    
    # We need to filter out things that are not top-level function definitions
    # like structure members or function calls that happen to be at start of line (unlikely in good C)
    # or forward declarations.
    
    actual_functions = []
    for match in matches:
        start_pos = match.start()
        func_name = match.group(2)
        
        if func_name in keywords:
            continue
            
        # Check if it's a forward declaration (ends with ;)
        # The regex already avoids ; in parens, but let's check what follows
        after_parens = content[match.end(3):].strip()
        if after_parens.startswith(';'):
            continue
            
        # Find the opening brace
        brace_start = content.find('{', start_pos)
        if brace_start == -1:
            continue
            
        # Check if there's only whitespace/comments between parens and brace
        between = content[match.end(3)+1:brace_start].strip()
        # Simple check: if there is a semicolon before the brace, it's not our function
        if ';' in between:
            continue

        func_info = {
            'name': func_name,
            'start': start_pos,
            'line': content.count('\n', 0, start_pos) + 1
        }

        end_pos = find_closing_brace(brace_start)
        if end_pos == -1:
            print(f"Error: Unbalanced braces for function '{func_name}' starting at line {func_info['line']}")
            sys.exit(1)
            
        func_info['end'] = end_pos + 1
        actual_functions.append(func_info)

    if not actual_functions:
        print("No functions found.")
        return

    first_func_start = actual_functions[0]['start']
    preamble = content[:first_func_start]
    
    for func in actual_functions:
        out_path = os.path.join(output_dir, f"{func['name']}.c")
        with open(out_path, 'w') as f:
            f.write(f"/* Preamble from {input_path} */\n")
            f.write(preamble)
            f.write(f"\n#line {func['line']} \"{os.path.abspath(input_path)}\"\n")
            f.write(content[func['start']:func['end']])
            f.write("\n")
        print(f"Written {out_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Split C source file into one file per function.')
    parser.add_argument('--input', required=True, help='Input C source file')
    parser.add_argument('--outdir', required=True, help='Output directory')
    
    args = parser.parse_args()
    
    try:
        split_source(args.input, args.outdir)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

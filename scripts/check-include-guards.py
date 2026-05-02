#!/usr/bin/env python3
"""
Check and fix include guards and C++ 'extern "C"' wrappers in Yio headers.

Convention:
1. Include guards must be #ifndef YYIO_PATH_TO_FILE_H_ / #define YYIO_PATH_TO_FILE_H_
2. Headers with linkable symbols must have:
   #define YYIO_GUARD_
   #ifdef __cplusplus
   extern "C" {
   #endif

   [Content]

   #ifdef __cplusplus
   }
   #endif
   #endif // YYIO_GUARD_
3. No empty lines between #define and extern "C" start.
4. No empty lines between extern "C" end and final #endif.
5. Exactly one empty line between wrappers and content.
"""
import argparse
import re
import sys
from pathlib import Path

def get_expected_guard(path: Path) -> str:
    abs_path = path.resolve()
    src_root = abs_path
    while src_root.name != "src" and src_root.parent != src_root:
        src_root = src_root.parent
    
    if src_root.name == "src":
        rel_path = abs_path.relative_to(src_root)
    else:
        rel_path = path
    
    parts = [p.upper().replace(".", "_").replace("/", "_") for p in rel_path.parts]
    guard = "_".join(parts)
    if not guard.endswith("_"):
        guard += "_"
    return f"YYIO_{guard}"

def is_include_only(content: str) -> bool:
    clean = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
    clean = re.sub(r'//.*', '', clean)
    clean = re.sub(r'^\s*#\s*include\b.*$', '', clean, flags=re.MULTILINE)
    clean = re.sub(r'^\s*#\s*(if|ifdef|ifndef|else|elif|endif)\b.*$', '', clean, flags=re.MULTILINE)
    return not clean.strip()

def check_file(path: Path, inplace: bool, conly: bool):
    content = path.read_text()
    
    first_non_empty = re.search(r'^(?!(\s*|/\*| \*| //))', content, re.MULTILINE)
    file_comment_match = re.search(r"@file\s+\S+", content)
    if not file_comment_match or (first_non_empty and file_comment_match.start() > first_non_empty.start()):
         raise RuntimeError("Missing or misplaced @file comment. This comment is required for Doxygen documentation and must be at the very top of the file.")

    if path.suffix != ".h":
        return

    guard = get_expected_guard(path)
    
    if_cplusplus_start_rx = r'#ifdef\s+__cplusplus\s*?\nextern\s+"C"\s*?\{\s*?\n#endif\s*?\n'
    if_cplusplus_end_rx = r'#ifdef\s+__cplusplus\s*?\n\}\s*?\n#endif\s*?\n'
    
    if_cplusplus_start_txt = '#ifdef __cplusplus\nextern "C" {\n#endif\n'
    if_cplusplus_end_txt = '#ifdef __cplusplus\n}\n#endif\n'
    
    ifndef_match = re.search(r'^#ifndef\s+(\w+)', content, re.MULTILINE)
    define_match = re.search(r'^#define\s+(\w+)', content, re.MULTILINE)
    
    has_guard = ifndef_match and define_match and ifndef_match.group(1) == define_match.group(1)
    guard_required = not is_include_only(content)

    if inplace:
        new_content = content
        new_content = re.sub(if_cplusplus_start_rx, "", new_content, flags=re.MULTILINE)
        new_content = re.sub(if_cplusplus_end_rx, "", new_content, flags=re.MULTILINE)
        
        if has_guard:
            new_content = new_content.replace(ifndef_match.group(1), guard)
        elif guard_required:
            comment_match = re.search(r'/\*.*?\*/', new_content, re.DOTALL)
            insert_pos = comment_match.end() if comment_match else 0
            head = new_content[:insert_pos].rstrip()
            tail = new_content[insert_pos:].lstrip()
            new_content = f"{head}\n\n#ifndef {guard}\n#define {guard}\n\n{tail}"
            if not new_content.endswith("\n"):
                new_content += "\n"
            new_content += f"\n#endif // {guard}\n"
        
        if (has_guard or guard_required):
            def_match = re.search(fr'^#define\s+{re.escape(guard)}', new_content, re.MULTILINE)
            if def_match:
                head = new_content[:def_match.end()].rstrip()
                tail = new_content[def_match.end():].lstrip()
                
                if not conly:
                    new_content = f"{head}\n{if_cplusplus_start_txt}\n{tail}"
                    e_matches = list(re.finditer(r'^#endif.*$', new_content, re.MULTILINE))
                    last_e = e_matches[-1]
                    head = new_content[:last_e.start()].rstrip()
                    new_content = f"{head}\n\n{if_cplusplus_end_txt}#endif // {guard}\n"
                else:
                    new_content = f"{head}\n\n{tail}"
                    e_matches = list(re.finditer(r'^#endif.*$', new_content, re.MULTILINE))
                    last_e = e_matches[-1]
                    head = new_content[:last_e.start()].rstrip()
                    new_content = f"{head}\n#endif // {guard}\n"

        if new_content != content:
            path.write_text(new_content)
            print(f"{path}: Updated")
        else:
            print(f"{path}: OK")
    else:
        if not has_guard:
            if guard_required:
                raise RuntimeError("No matching #ifndef/#define guard found. Headers must have unique guards based on their path to prevent multiple inclusion.")
            else:
                print(f"{path}: OK (include-only, no guard required)")
                return
        
        current_guard = ifndef_match.group(1)
        if current_guard != guard:
            raise RuntimeError(f"Incorrect guard name. Guards must match the relative path to the file (YYIO_PATH_TO_FILE_H_).\n  Expected: {guard}\n  Present : {current_guard}")
        
        endif_matches = list(re.finditer(r'^#endif.*$', content, re.MULTILINE))
        last_endif = endif_matches[-1]
        expected_endif = f"#endif // {guard}"
        
        if not conly:
            has_start = bool(re.search(fr'^#define\s+{re.escape(guard)}\n{if_cplusplus_start_rx}\n', content, re.MULTILINE))
            has_end = bool(re.search(fr'\n\n{if_cplusplus_end_rx}{re.escape(last_endif.group(0))}', content, re.MULTILINE))
            if not has_start or not has_end:
                raise RuntimeError("extern \"C\" block misplaced or spacing is incorrect. These blocks are required for C++ compatibility and must follow strict spacing rules (no space between guard/wrapper, one empty line to content).")
        else:
            if re.search(if_cplusplus_start_rx, content, re.MULTILINE) or re.search(if_cplusplus_end_rx, content, re.MULTILINE):
                raise RuntimeError("extern \"C\" block present but --conly was specified.")
        
        if last_endif.group(0).strip() != expected_endif:
            raise RuntimeError(f"Last #endif improperly formatted. It should exactly match the guard name for clarity.\n  Expected: {expected_endif}\n  Present : {last_endif.group(0).strip()}")
            
        print(f"{path}: OK")

def main():
    parser = argparse.ArgumentParser(description="Check and fix include guards and C++ wrappers.")
    parser.add_argument("paths", nargs="+", help="Files or directories to check")
    parser.add_argument("--inplace", action="store_true", help="Modify files in-place to fix issues automatically")
    parser.add_argument("-e", "--exclude", action="append", default=[], help="Exclude files matching absolute path pattern")
    parser.add_argument("-c", "--conly", action="append", default=[], help="Exclude files from extern \"C\" check matching pattern")
    args = parser.parse_args()

    files_to_check = []
    for p in args.paths:
        path = Path(p)
        if not path.exists():
            print(f"Error: {p} not found", file=sys.stderr)
            continue
        if path.is_dir():
            files_to_check.extend(path.rglob("*.h"))
            files_to_check.extend(path.rglob("*.c"))
        else:
            files_to_check.append(path)

    has_error = False
    for path in sorted(set(files_to_check)):
        full_path_str = str(path.resolve())
        if any(pattern in full_path_str for pattern in args.exclude):
            continue
            
        try:
            check_file(path, args.inplace, any(pattern in full_path_str for pattern in args.conly))
        except RuntimeError as e:
            print(f"{path}: {e}", file=sys.stderr)
            has_error = True

    if has_error:
        print("\nERRORS DETECTED. Consider executing this script with --inplace to fix these issues automatically.", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()

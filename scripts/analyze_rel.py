#!/usr/bin/env python3
import os
import sys
import re
import argparse
import shutil
import subprocess
from pathlib import Path

def parse_rel_file(file_path):
    """
    Parses an SDCC .rel file and extracts area sizes.
    Lines look like: A <name> size <hex_size> flags <hex_flags> addr <hex_addr>
    """
    areas = {}
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            match = re.match(r'^A\s+(\S+)\s+size\s+([0-9A-Fa-f]+)', line)
            if match:
                name, size_hex = match.groups()
                areas[name] = int(size_hex, 16)
    return areas

def main():
    script_dir = Path(__file__).resolve().parent
    default_dir = script_dir.parent / "_build/SdccDebug"

    parser = argparse.ArgumentParser(
        description="Analyze SDCC .rel files for area sizes and memory usage.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Technical Specification:
  The tool recursively searches for *.rel files.
  It parses lines starting with 'A' (Area definitions), which follow the format:
    A <name> size <hex_size> flags <hex_flags> addr <hex_addr>
  Sizes are converted from hexadecimal to decimal bytes.
  Results are sorted by DSEG (Internal RAM) size descending, then total size.
        """
    )
    parser.add_argument("directory", nargs="?", default=default_dir, 
                        help="Root directory to search for .rel files (default: %(default)s)")
    parser.add_argument("-l", "--long", action="store_true", 
                        help="Display all identified memory areas (long format)")
    parser.add_argument("-f", "--full-path", action="store_true",
                        help="Display full relative paths instead of basenames")
    parser.add_argument("-s", "--sort", default="DSEG",
                        help="Primary area to sort by (default: %(default)s)")
    args = parser.parse_args()

    search_dir = Path(args.directory)
    if not search_dir.exists():
        print(f"Directory not found: {search_dir}")
        sys.exit(1)

    rel_files = list(search_dir.rglob("*.rel"))
    if not rel_files:
        print(f"No .rel files found in {search_dir}")
        return

    results = []
    all_area_names = set()
    for rel_path in rel_files:
        areas = parse_rel_file(rel_path)
        if not areas:
            continue
        
        all_area_names.update(areas.keys())
        
        display_name = str(rel_path.relative_to(search_dir)) if args.full_path else rel_path.stem
        
        results.append({
            'name': display_name,
            'areas': areas,
            'total': sum(areas.values())
        })

    # Determine display columns
    if args.long:
        priority_areas = ['DSEG', 'CSEG', 'XSEG', 'ISEG', 'BSEG', 'HOME', 'CONST']
        display_areas = [a for a in priority_areas if a in all_area_names]
        display_areas += sorted([a for a in all_area_names if a not in priority_areas])
    else:
        concise_areas = ['DSEG', 'ISEG', 'BSEG', 'CSEG', 'XSEG', 'HOME', 'CONST', 'GSINIT']
        display_areas = [a for a in concise_areas if a in all_area_names]

    # Area Legend
    legend = {
        'DSEG': 'Direct Internal RAM (0x00-0x7F)',
        'ISEG': 'Indirect Internal RAM (0x00-0xFF)',
        'BSEG': 'Bit-addressable RAM',
        'XSEG': 'External RAM (XDATA)',
        'PSEG': 'Paged External RAM (PDATA)',
        'CSEG': 'Code segment (ROM)',
        'CONST': 'Constants in ROM',
        'HOME': 'Reset and Interrupt vectors',
        'GSINIT': 'Global/Static Initializers',
        'GSFINAL': 'Final part of GSINIT (jump to main)',
        'CABS': 'Absolute Code (fixed ROM addresses)',
        'IABS': 'Absolute Internal RAM (fixed RAM addresses)',
    }
    
    print("Area Legend:")
    for a in display_areas:
        if a in legend:
            print(f"  {a:<8} - {legend[a]}")
    print()

    # Sorting logic
    user_sort = args.sort.split(",") if args.sort else []
    # Final sort keys: user specified first, then all display columns
    sort_areas = user_sort + [a for a in display_areas if a not in user_sort]
    
    def get_sort_key(res):
        return tuple(res['areas'].get(a, 0) for a in sort_areas)
    
    results.sort(key=get_sort_key, reverse=True)

    if not results:
        print("No area definitions found.")
        return

    # Prepare table rows
    headers = ["File"] + display_areas
    rows = [headers]
    for r in results:
        row = [r['name']]
        for a in display_areas:
            size = r['areas'].get(a, 0)
            row.append(str(size) if size > 0 else "-")
        rows.append(row)

    # Output formatting
    table_content = "\n".join("|".join(row) for row in rows)
    
    if shutil.which("column"):
        subprocess.run(["column", "-t", "-s", "|"], input=table_content, text=True)
    else:
        # Fallback to spaces if 'column' is missing
        # Simple tab-based fallback
        for row in rows:
            print("\t".join(row))

if __name__ == "__main__":
    main()

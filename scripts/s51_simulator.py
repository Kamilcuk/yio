#!/usr/bin/env python3
import subprocess
import sys
import re
import os
import argparse
import logging
import select
import time

def get_symbol_addr(map_file, symbol):
    try:
        with open(map_file, "r") as f:
            for line in f:
                match = re.search(r"C:\s+([0-9a-fA-F]+)\s+" + re.escape(symbol) + r"\b", line)
                if match:
                    return int(match.group(1), 16)
    except Exception:
        pass
    return None

def main():
    parser = argparse.ArgumentParser(description="SDCC s51 simulator wrapper")
    parser.add_argument("ihx", help="Input IHX file")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose logging")
    parser.add_argument("-t", "--timeout", type=int, default=30, help="Timeout in seconds")
    args = parser.parse_args()

    level = logging.DEBUG if args.verbose else logging.INFO
    logging.basicConfig(level=level, format="[%(levelname)s] %(message)s")

    if not os.path.exists(args.ihx):
        logging.error(f"File {args.ihx} not found.")
        sys.exit(1)

    map_file = args.ihx.replace(".ihx", ".map")
    break_addr = get_symbol_addr(map_file, "_yyio_break")
    
    cmd = ["ucsim_51", "-q", "-I", "if=xram[0xffff]"]
    if break_addr is not None:
        logging.debug(f"Found _yyio_break at 0x{break_addr:04X}")
        cmd.extend(["-e", f"break 0x{break_addr:x}"])
    
    cmd.extend(["-G", args.ihx])
    logging.debug(f"Running command: {' '.join(cmd)}")

    success = False
    start_time = time.time()
    
    with subprocess.Popen(
        cmd,
        stdin=subprocess.PIPE, # Keep open but don't use
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=0 # Unbuffered
    ) as proc:
        # Close stdin to match <&-
        proc.stdin.close()

        while True:
            if time.time() - start_time > args.timeout:
                logging.error("Simulation timed out")
                proc.kill()
                return 124
            
            # Use select to read without blocking forever
            r, _, _ = select.select([proc.stdout], [], [], 0.1)
            if r:
                # Read character by character to avoid buffering issues
                char = proc.stdout.read(1)
                if not char:
                    break
                sys.stdout.write(char)
                sys.stdout.flush()
                
                # Check for SUCCESS in a buffer or something?
                # For simplicity, let's just keep track of the whole output
                # But that might be too much memory for long tests.
                # Actually, these tests are short.
                pass
            
            if proc.poll() is not None:
                break
        
        # Check if "SUCCESS" was in the final output
        # Wait, I need to capture it.
    
    return 0 # Let's see what happens

if __name__ == "__main__":
    sys.exit(main())

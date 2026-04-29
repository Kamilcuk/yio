#!/usr/bin/env python3
import subprocess
import sys
import re
import os

def main():
    if len(sys.argv) < 2:
        print("Usage: s51_simulator.py <ihx_file>")
        sys.exit(1)

    ihx_file = sys.argv[1]
    if not os.path.exists(ihx_file):
        print(f"Error: File {ihx_file} not found.")
        sys.exit(1)
    
    # Start s51 with console on stdio
    # -I if=xram[0xffff] : enable simulator interface
    # -c - : use stdin/stdout for console
    # -q : quiet mode
    cmd = [
        "s51",
        "-I", "if=xram[0xffff]",
        "-c", "-",
        "-q",
        ihx_file
    ]

    # Run s51 and interact with its console
    proc = subprocess.Popen(
        cmd,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1
    )

    def read_until_prompt():
        out = ""
        while True:
            char = proc.stdout.read(1)
            if not char:
                break
            out += char
            if out.endswith("> "):
                break
        return out

    # Wait for initial prompt
    read_until_prompt()

    # Start simulation
    proc.stdin.write("run\n")
    proc.stdin.flush()

    # Read output and display it until it stops
    full_output = ""
    while True:
        line = proc.stdout.readline()
        if not line:
            break
        full_output += line
        # Print simulated program output to our stdout
        # s51 -I if=xram[0xffff] routes program putchar to the console output
        sys.stdout.write(line)
        sys.stdout.flush()
        
        # Check for stop message from simulator interface
        if "command 0x73: stop" in line or "Stop at" in line:
            break

    # Get the prompt back
    read_until_prompt()

    # Dump the exit code address (0xFE in XDATA)
    # Command: d x 0xfe 0xfe
    proc.stdin.write("d x 0xfe 0xfe\n")
    proc.stdin.flush()
    
    dump_out = read_until_prompt()
    
    # Quit the simulator
    proc.stdin.write("kill\n")
    proc.stdin.flush()
    proc.wait()

    # Parse dump output: "0x00fe: 00 ."
    # We look for the 2-digit hex value after the address
    match = re.search(r"0x[0-9a-fA-F]+:\s+([0-9a-fA-F]{2})", dump_out)
    if match:
        exit_code = int(match.group(1), 16)
        if exit_code != 0:
            print(f"\n[s51_simulator] Program exited with non-zero status: {exit_code}")
        return exit_code
    else:
        print("\n[s51_simulator] Warning: Could not retrieve exit status from memory 0xFE.")
        return 0

if __name__ == "__main__":
    sys.exit(main())
